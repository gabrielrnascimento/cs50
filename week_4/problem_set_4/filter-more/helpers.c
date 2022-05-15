#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
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
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        int k = 0;
        for (int j = width - 1; j >= 0; j--)
        {
            temp[i][k] = image[i][j];
            k++;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
// ? create copy
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

// ? blur
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int box[3] = {-1, 0, 1};
            float pixel_count = 0.0;
            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    int x = i + box[k];
                    int y = j + box[l];
                    if (((x >= 0) && (x < height)) && ((y >= 0) && (y < width)))
                    {
                        sum_red = sum_red + temp[x][y].rgbtRed;
                        sum_green = sum_green + temp[x][y].rgbtGreen;
                        sum_blue = sum_blue + temp[x][y].rgbtBlue;
                        pixel_count = pixel_count + 1.0;
                    }
                }
            }
            image[i][j].rgbtRed = round(sum_red / pixel_count);
            image[i][j].rgbtGreen = round(sum_green / pixel_count);
            image[i][j].rgbtBlue = round(sum_blue / pixel_count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
// ? create copy
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

// ? edge
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int box[3] = {-1, 0, 1};
            int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
            int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            int sum_red_x = 0;
            int sum_red_y = 0;

            int sum_green_x = 0;
            int sum_green_y = 0;

            int sum_blue_x = 0;
            int sum_blue_y = 0;

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    int x = i + box[k];
                    int y = j + box[l];

                    if (((x >= 0) && (x < height)) && ((y >= 0) && (y < width)))
                    {
                        sum_red_x = sum_red_x + (temp[x][y].rgbtRed * gx[k][l]);
                        sum_red_y = sum_red_y + (temp[x][y].rgbtRed * gy[k][l]);

                        sum_green_x = sum_green_x + (temp[x][y].rgbtGreen * gx[k][l]);
                        sum_green_y = sum_green_y + (temp[x][y].rgbtGreen * gy[k][l]);

                        sum_blue_x = sum_blue_x + (temp[x][y].rgbtBlue * gx[k][l]);
                        sum_blue_y = sum_blue_y + (temp[x][y].rgbtBlue * gy[k][l]);
                    }
                }
            }

            int total_red = round(sqrt(pow(sum_red_x, 2) + pow(sum_red_y, 2)));
            int total_green = round(sqrt(pow(sum_green_x, 2) + pow(sum_green_y, 2)));
            int total_blue = round(sqrt(pow(sum_blue_x, 2) + pow(sum_blue_y, 2)));

            if (total_red > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = total_red;
            }
            if (total_green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = total_green;
            }
            if (total_blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = total_blue;
            }
        }
    }
    return;
}
