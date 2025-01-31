#include <cs50.h>
#include <stdio.h>
#define MAX 8
#define MIN 1

void print_heshes(int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Nomber 1-8: ");
    }
    while (n < MIN || n > MAX);

    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            printf(" ");
        }
        print_heshes(i);
        printf("\n");
    }
}

void print_heshes(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
    printf("  ");
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}
