#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: ");
    int l = strlen(text);
    int cherectars = 0, stancsis = 0, words = 1;
    for (int i = 0; i < l; i++)
    {
        if (isalpha(text[i]))
        {
            cherectars++;
        }
        else if (text[i] == ' ')
        {
            words++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            stancsis++;
        }
    }
    float L, S, index;
    L = (float) (cherectars * 100) / (words);
    S = (float) (stancsis * 100) / (words);
    index = 0.0588 * L - 0.296 * S - 15.8;

    if (index < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    index = round(index);
    if (index > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    printf("Grade %.0f\n", index);
}
