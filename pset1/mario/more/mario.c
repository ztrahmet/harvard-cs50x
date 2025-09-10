#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 23);

    for (int i = 1; i <= height; i++)
    {
        // Spaces at front to left align
        for (int j = 0; j < height - i; j++)
        {
            printf(" ");
        }
        // Left aligned #
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        // Space in between
        printf("  ");
        // Right aligned #
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        // New line
        printf("\n");
    }
}
