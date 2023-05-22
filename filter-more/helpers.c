#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //loop through all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //compute average and set RGB values to it
            float sum = (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen);
            BYTE avg = round(sum / 3.0);

            image[i][j].rgbtBlue = round(avg);
            image[i][j].rgbtRed = round(avg);
            image[i][j].rgbtGreen = round(avg);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        //only swap n/2 times, otherwise, we will get original image
        for (int j = 0, first = 0, last = width - 1; j < width / 2; j++, first++, last--)
        {
            //swap
            RGBTRIPLE temp = image[i][first];
            image[i][first] =  image[i][last];
            image[i][last] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create a copy of the bitmap
    RGBTRIPLE avg[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg[i][j] = image[i][j];
        }
    }

    //looping through all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //variable for keeping track of how many pixels surround current pixel
            int num_surrounding = 0;
            //variables for finding the average RGB of surrounding pixels
            float red = 0;
            float green = 0;
            float blue = 0;

            //nested loop will look at the pixels surrounding current
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    //if the pixel exists, add its R, G, and B value to corresponding variable
                    if (i + k >= 0 && i + k < height)
                    {
                        if (j + l >= 0 && j + l < width)
                        {
                            red += avg[i + k][j + l].rgbtRed;
                            blue += avg[i + k][j + l].rgbtBlue;
                            green += avg[i + k][j + l].rgbtGreen;
                            num_surrounding++;
                        }
                    }
                }
            }
            //store average into original pixel
            image[i][j].rgbtRed = round(red / num_surrounding);
            image[i][j].rgbtGreen = round(green / num_surrounding);
            image[i][j].rgbtBlue = round(blue / num_surrounding);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Initialize 2D Arrays for Gx and Gy values
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //copy of bitmap
    RGBTRIPLE edge[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edge[i][j] = image[i][j];
        }
    }

    //loop through all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            //variables to store Gx and Gy values
            float gxRed = 0;
            float gxGreen = 0;
            float gxBlue = 0;

            float gyRed = 0;
            float gyGreen = 0;
            float gyBlue = 0;

            //nested loop to look at surrounding pixels
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    //checks if pixel exists
                    if (i + k >= 0 && i + k < height)
                    {
                        //calculates and stores gx and gy values
                        if (j + l >= 0 && j + l < width)
                        {
                            gxRed += edge[i + k][j + l].rgbtRed * gx[k + 1][l + 1];
                            gxBlue += edge[i + k][j + l].rgbtBlue * gx[k + 1][l + 1];
                            gxGreen += edge[i + k][j + l].rgbtGreen * gx[k + 1][l + 1];

                            gyRed += edge[i + k][j + l].rgbtRed * gy[k + 1][l + 1];
                            gyBlue += edge[i + k][j + l].rgbtBlue * gy[k + 1][l + 1];
                            gyGreen += edge[i + k][j + l].rgbtGreen * gy[k + 1][l + 1];
                        }
                    }
                }
            }

            //combines gx and gy values per the algorithm and makes sure its value <= 255
            float channel_val_r = (sqrt(gxRed * gxRed + gyRed * gyRed));
            int red = round(channel_val_r);
            if (red > 255)
            {
                red = 255;
            }
            float channel_val_g = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int green = round(channel_val_g);
            if (green > 255)
            {
                green = 255;
            }
            int channel_val_b = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
            int blue = round(channel_val_b);
            if (blue > 255)
            {
                blue = 255;
            }

            //changes original pixel to new value
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}
