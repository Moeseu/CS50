#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

bool vote(int voter, int rank, string name);
void tabulate(void);
int find_min(void);
bool print_winner(void);
bool is_tie(int min);
void eliminate(int min);

typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

candidate candidates[MAX];
int preferences[MAX][MAX];
int candidate_count;
int voter_count;

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;

    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].eliminated = false;
        candidates[i].votes = 0;
    }

    voter_count = get_int("Number of voters: ");

    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
    }

    while (true)
    {
        tabulate();

        if (print_winner())
        {
            return 0;
        }

        int min = find_min();

        if (is_tie(min))
        {
            return 0;
        }

        eliminate(min);

        // Reset votes for the next round
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
}

bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }

    return false;
}

void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int candidate = preferences[i][j];
            if (!candidates[candidate].eliminated)
            {
                candidates[candidate].votes++;
                break;
            }
        }
    }
}

bool print_winner(void)
{
    int majority = voter_count / 2;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > majority)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }

    return false;
}

int find_min(void)
{
    int min_votes = candidates[0].votes;

    for (int i = 1; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min_votes)
        {
            min_votes = candidates[i].votes;
        }
    }

    return min_votes;
}

bool is_tie(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            return false;
        }
    }

    return true;
}

void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}
