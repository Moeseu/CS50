#include "helpers.h"
#include <math.h>

typedef uint8_t BYTE;

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            int avg = round(((double)image[i][j].rgbtRed + (double)image[i][j].rgbtGreen + (double)image[i][j].rgbtBlue) / 3);

            // Update pixel values
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

int max_color(int color);

void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            image[i][j].rgbtRed = max_color(round((.393 * (double)image[i][j].rgbtRed) + (.769 * (double)image[i][j].rgbtGreen) + (.189 * (double)image[i][j].rgbtBlue)));
            image[i][j].rgbtGreen = max_color(round((.349 * (double)image[i][j].rgbtRed) + (.686 * (double)image[i][j].rgbtGreen) + (.168 * (double)image[i][j].rgbtBlue)));
            image[i][j].rgbtBlue = max_color(round((.534 * (double)image[i][j].rgbtRed) + (.534 * (double)image[i][j].rgbtGreen) + (.131 * (double)image[i][j].rgbtBlue)));
        }
    }
}

int max_color(int color)
{
    if(color > 255)
    {
        return 255;
    }
    return color;
}
