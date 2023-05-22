#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //goes through every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        //if name matches, increase votes by 1
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //array to keep track of all winners, since ties can happen
    string winners[candidate_count];

    //finds the highest score and saves it into an int
    int highest_votes = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > highest_votes)
        {
            highest_votes = candidates[i].votes;
        }
    }

    //int to keep track of number of winners
    int winners_counter = 0;

    //go through every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        //if candidate has the highest vote, add their name to array and increase winner counter
        if (candidates[i].votes == highest_votes)
        {
            winners[winners_counter] = candidates[i].name;
            winners_counter++;
        }
    }

    // print all winners
    for (int i = 0; i < winners_counter; i++)
    {
        printf("%s\n", winners[i]);
    }

    return;
}