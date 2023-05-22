#include <stdio.h>
#include <cs50.h>

//will print the "blocks" in the mario game
void print_blocks(int current_row, int height)
{
    for (int i = 0; i <= height - current_row; i++)
    {
        printf("#");
    }
}

int main(void)
{
    //keep prompting for a number until it's between 1 and 8
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);


    //for every row
    for (int i = height; i > 0; i--)
    {
        //prints spaces
        for (int j = i - 1; j > 0; j--)
        {
            printf(" ");
        }

        print_blocks(i, height);
        printf("  "); //prints double space between blocks
        print_blocks(i, height);

        printf("\n"); //new row
    }
}