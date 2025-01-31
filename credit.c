#include <cs50.h>
#include <stdio.h>

int count(long nomber);
int sumdig(int nom);
int firsttwo(long nom);

int main(void)
{
    long nomber;
    int sum = 0;
    // get nomber
    do
    {
        nomber = get_long("The cart`s nomber is ");
    }
    while (nomber < 0);

    int digits = count(nomber);
    // Can pay
    for (long i = nomber; i > 0; i /= 10)
    {
        sum += i % 10;
        i /= 10;
        sum += sumdig(2 * (i % 10));
    }
    int first = firsttwo(nomber);
    if (sum % 10 == 0)
    {
        if (digits == 15 && (first == 34 || first == 37))
        {
            printf("AMEX\n");
        }
        else if (digits == 16 && first > 50 && first < 56)
        {
            printf("MASTERCARD\n");
        }
        else if ((digits == 16 && first / 10 == 4) || (digits == 13 && first / 10 == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
// count digits
int count(long nomber)
{
    int digits = 0;
    do
    {
        nomber = nomber / 10;
        digits++;
    }
    while (nomber > 0);

    return digits;
}
// count sum of digits
int sumdig(int nom)
{
    int sum = 0;
    do
    {
        sum += nom % 10;
        sum += nom / 10;
    }
    while (nom < 0);

    return sum;
}

int firsttwo(long nom)
{
    while (nom > 99)
    {
        nom = nom / 10;
    }
    return nom;
}
