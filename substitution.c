#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define LETTERS 26

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    if (strlen(argv[1]) != LETTERS)
    {
        printf("Key must contain 26 characters");
        return 1;
    }

    for (int i = 0; i < LETTERS; i++)
    {
        if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
        {
            argv[1][i] = tolower(argv[1][i]);
        }

        if (argv[1][i] < 'a' || argv[1][i] > 'z')
        {
            printf("Key must only contain alphabetic characters");
            return 1;
        }
    }

    for (int i = 0; i < LETTERS - 1; i++)
    {
        for (int j = i + 1; j < LETTERS; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must not contain repeated characters");
                return 1;
            }
        }
    }

    string text = get_string("Enter your text: ");
    bool isupper = false;
    int nom;

    printf("ciphertext: ");
    for (int i = 0; i < strlen(text); i++)
    {

        isupper = false;

        if (text[i] <= 'Z' && text[i] >= 'A')
        {
            isupper = true;
            text[i] = tolower(text[i]);
        }
        if (text[i] <= 'z' && text[i] >= 'a')
        {
            nom = (int) text[i] - (int) 'a';
            text[i] = argv[1][nom];

            if (isupper)
            {
                text[i] = toupper(argv[1][nom]);
            }
        }

        printf("%c", text[i]);
    }
    printf("\n");
}
