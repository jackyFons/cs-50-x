// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 5000;

unsigned int word_counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //looks at first element
    node *n = table[hash(word)];

    //checks whether word matches
    while (n != NULL)
    {
        if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    //adds all of the ASCII char in the word
    unsigned int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += toupper(word[i]);
    }

    sum = sum % N;
    return sum;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //open file
    FILE *inFile = fopen(dictionary, "r");
    if (inFile == NULL)
    {
        return false;
    }

    //reads file
    char buffer[LENGTH + 1];

    while (fscanf(inFile, "%s", buffer) != EOF)
    {
        //creates new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(n);
            return false;
        }
        //gets hash value
        unsigned int index = hash(buffer);

        //add to hash table
        strcpy(n->word, buffer);
        n->next = table[index];
        table[index] = n;

        //increase counter
        word_counter++;
    }
    //close file
    fclose(inFile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        //if there are no elements, ignore
        if (table[i] == NULL)
        {
            continue;
        }
        //frees memory
        while (table[i] != NULL)
        {
            node *temp = table[i];
            node *traverse = table[i]->next;

            table[i] = traverse;
            free(temp);
        }
    }
    return true;
}
