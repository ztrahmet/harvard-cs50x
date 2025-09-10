#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open input file
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    uint8_t buffer[512];
    FILE *img = NULL;
    int filecount = 0;
    char filename[8]; // Enough for "###.jpg\0"

    // Read blocks of 512 bytes
    while (fread(buffer, sizeof(uint8_t), 512, card) == 512)
    {
        // Check for JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If a JPEG is already open, close it
            if (img != NULL)
            {
                fclose(img);
            }

            // Create new filename
            sprintf(filename, "%03i.jpg", filecount);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create output JPEG.\n");
                fclose(card);
                return 1;
            }

            filecount++;
        }

        // If a JPEG file is open, write to it
        if (img != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), 512, img);
        }
    }

    // Clean up
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);
    return 0;
}
