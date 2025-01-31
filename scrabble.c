#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int winner(string player);
string low(string play);

int main(void)
{
    string player1 = get_string("Player 1: ");
    string player2 = get_string("Player 2: ");
    player1 = low(player1);
    player2 = low(player2);
    ;
    int val1 = winner(player1);
    int val2 = winner(player2);
    if (val1 > val2)
    {
        printf("Player 1 wins!\n");
    }
    else if (val1 < val2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int winner(string player)
{
    int n = 0;
    int l = strlen(player);
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (player[i] == j + 'a')
            {
                n += points[j];
            }
        }
    }
    return n;
}

string low(string play)
{
    int l = strlen(play);
    for (int i = 0; i < l; i++)
    {
        play[i] = tolower(play[i]);
    }
    return play;
}
