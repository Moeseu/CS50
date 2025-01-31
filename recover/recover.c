#include <cs50.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    BYTE buffer[BUFFER_SIZE];
    int filecount = 0;
    FILE *picture = NULL;
    bool jpg_found = false;

    while (fread(buffer, sizeof(BYTE), BUFFER_SIZE, input))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (jpg_found)
            {
                fclose(picture);
            }
            else
            {
                jpg_found = true;
            }

            char filename[8];
            sprintf(filename, "%03i.jpg", filecount);
            picture = fopen(filename, "w");
            if (picture == NULL)
            {
                printf("Could not create file %s.\n", filename);
                fclose(input);
                return 1;
            }

            filecount++;
        }

        if (jpg_found)
        {
            fwrite(buffer, sizeof(BYTE), BUFFER_SIZE, picture);
        }
    }

    fclose(input);
    if (picture != NULL)
    {
        fclose(picture);
    }

    return 0;
}
