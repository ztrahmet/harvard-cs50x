#include "helpers.h"
#include <math.h> // round()

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed, green = image[i][j].rgbtGreen,
                 blue = image[i][j].rgbtBlue, avg = round((red + green + blue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    const int BLUR_SIZE = 3;

    RGBTRIPLE copy[height][width];

    // Copy buffer to prevent interference between updated and original pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Apply blur to each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redTotal = 0, greenTotal = 0, blueTotal = 0, count = 0;

            // Grid BLUR_SIZE x BLUR_SIZE
            for (int gi = i - (BLUR_SIZE / 2); gi <= i + (BLUR_SIZE / 2); gi++)
            {
                for (int gj = j - (BLUR_SIZE / 2); gj <= j + (BLUR_SIZE / 2); gj++)
                {
                    // Check corner
                    if (gi >= 0 && gi < height && gj >= 0 && gj < width)
                    {
                        redTotal += copy[gi][gj].rgbtRed;
                        greenTotal += copy[gi][gj].rgbtGreen;
                        blueTotal += copy[gi][gj].rgbtBlue;
                        count++;
                    }
                }
            }

            // Assign average color
            image[i][j].rgbtRed = round(redTotal / (float) count);
            image[i][j].rgbtGreen = round(greenTotal / (float) count);
            image[i][j].rgbtBlue = round(blueTotal / (float) count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    const int SIZE = 3;

    // Sobel kernels
    int Gx[SIZE][SIZE] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int Gy[SIZE][SIZE] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Create a copy of the image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Apply Sobel operator
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxRed = 0, gyRed = 0;
            int gxGreen = 0, gyGreen = 0;
            int gxBlue = 0, gyBlue = 0;

            // Loop over 3x3 grid
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        int weightX = Gx[di + 1][dj + 1];
                        int weightY = Gy[di + 1][dj + 1];

                        RGBTRIPLE pixel = copy[ni][nj];

                        gxRed += pixel.rgbtRed * weightX;
                        gyRed += pixel.rgbtRed * weightY;
                        gxGreen += pixel.rgbtGreen * weightX;
                        gyGreen += pixel.rgbtGreen * weightY;
                        gxBlue += pixel.rgbtBlue * weightX;
                        gyBlue += pixel.rgbtBlue * weightY;
                    }
                }
            }

            // Compute magnitude of gradient
            int red = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            // Clamp to 255
            image[i][j].rgbtRed = red > 255 ? 255 : red;
            image[i][j].rgbtGreen = green > 255 ? 255 : green;
            image[i][j].rgbtBlue = blue > 255 ? 255 : blue;
        }
    }
}
