#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

/*checks if all characters in key are used more than once or if a
    non alphabetic character is used*/
int check_characters(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = toupper(s[i]);
        //checks if character is alphabetic
        if (s[i] < 65 || s[i] > 90)
        {
            printf("Error: A character in the key is not alphabetic.\n");
            return 1;
        }
    }

    const int COUNTER_LEN = 26;
    int counter[26] = {0}; //array to keep track of how many times a character has been used

    for (int j = 0; j < strlen(s); j++)
    {
        //increases counter for letter used
        counter[s[j] - 65]++;
    }

    //checks to see if every letter has been used once
    for (int k = 0; k < COUNTER_LEN; k++)
    {
        if (counter[k] != 1)
        {
            printf("Error: Not every alphabetic character is used once.\n");
            return 1;
        }
    }
    return 0;
}

string cipher(string key, string plain)
{
    string ciphered = plain;
    //goes through every character in plain text
    for (int i = 0; i < strlen(plain); i++)
    {
        //leaves non alphabetical characters alone
        if (toupper(plain[i]) <  65 || toupper(plain[i]) > 90)
        {
            ciphered[i] = plain[i];
        }
        //keeps uppercase characters uppercase
        else if (plain[i] >=  65 && plain[i] <= 90)
        {
            int index = plain[i] - 65;
            ciphered[i] = toupper(key[index]);
        }
        //keeps lowercase characters lowercase
        else
        {
            int index = plain[i] - 97;
            ciphered[i] = tolower(key[index]);
        }
    }

    return ciphered;
}

int main(int argc, string argv[])
{
    //check if there is an argument
    if (argc != 2)
    {
        printf("Error: no command-line argument.\n");
        return 1;
    }

    //check if key has length of 26
    if (strlen(argv[1]) != 26)
    {
        printf("Error: Length is not 26.\n");
        return 1;
    }

    //check if key's characters are valid using created function
    if (check_characters(argv[1]))
    {
        return 1;
    }

    string plain_text = get_string("plaintext: "); //prompt
    string cipher_text = cipher(argv[1], plain_text); //creates ciphered text
    printf("ciphertext: %s", cipher_text); //output
    printf("\n");
    return 0; //successful
}