// Implements a dictionary's functionality

#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
unsigned int count_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int h = hash(word);
    node *cusor = table[h];
    while (cusor != NULL)
    {
        if (strcasecmp(cusor->word, word) == 0)
        {
            return true;
        }
        cusor = cusor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // return toupper(word[0]) - 'A';
    return toupper(word[0]) - 'A' + strlen(word);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    char buffer[LENGTH + 1];
    int hash_index = 0;

    while (fscanf(file, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
            break;
        }

        strcpy(n->word, buffer);
        n->next = NULL;

        hash_index = hash(buffer);

        if (table[hash_index] == NULL)
        {
            table[hash_index] = n;
        }

        else
        {
            n->next = table[hash_index];
            table[hash_index] = n;
        }
        count_words++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cur1;
    node *cur2;
    for (int i = 0; i < N; i++)
    {
        cur1 = table[i];
        while (cur1 != NULL)
        {
            cur2 = cur1->next;
            free(cur1);
            ;
            cur1 = cur2;
        }
    }
    return true;
}
