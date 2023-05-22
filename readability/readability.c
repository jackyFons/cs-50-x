#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    float num_words = count_words(text);

    float avg_letters = count_letters(text) / num_words * 100.0; //gets avg letters per 100 words
    float avg_sentences = count_sentences(text) / num_words * 100.0; //gets avg entences per 100 words

    int index =  round(0.0588 * avg_letters - 0.296 * avg_sentences - 15.8); //gets index, then rounds

    //output depending on index
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

//will count the number of letters in the sentence
int count_letters(string text)
{
    int count = 0;
    for (int i = 0; i < strlen(text); i++) //goes through every character
    {
        //if it's a letter, increase counter
        if (toupper(text[i]) >= 65 && toupper(text[i]) <= 90)
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int count = 0;
    for (int i = 0; i < strlen(text); i++) //goes through every character
    {
        //if it's a space, increase counter
        if (text[i] == ' ')
        {
            count++;
        }
    }
    count++; //last word that doesnt have a space after
    return count;
}

int count_sentences(string text)
{
    int count = 0;
    for (int i = 0; i < strlen(text); i++) //goes through every character
    {
        //if it's a ., ?, or !, increase counter
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}