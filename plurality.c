#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define MAX 9

/* bool vote(string name);
void print_winner(void); */

typedef struct
{
    string name;
    int votes;
}candidate;

int main(int argc, string argv[])
{
    candidate candidates[argc];
    for(int i = 1; i < argc; i++)
    {
        candidates[i].name = argv[i];
        candidates[i].votes = 0;
    }

    int max = 0;
    int voters = get_int("Nomber of voters: ");
    if(voters > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 1;
    }
    string votefor;

    for(int j = 1; j <= voters; j++)
    {
        votefor = get_string("Vote: ");
        for(int i = 1; i < argc; i++)
        {
            if(!strcmp(candidates[i].name, votefor))
            {
                candidates[i].votes++;
                printf("for %s\n", candidates[i].name);
            }
            else
            {
                printf("Invalid vote.\n");
            }
        }
    }

     for(int i = 1; i < argc; i++)
     {
        if(candidates[i].votes > max){
            max = candidates[i].votes;
        }
     }

    for(int i = 1; i < argc; i++)
    {
        if(candidates[i].votes == max){
            printf("%s\n", candidates[i].name);
        }
    }
}

/* bool vote(string name)
{
    printf("%s\n", candidates[1].name);
}
 */
