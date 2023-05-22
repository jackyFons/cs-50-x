#include <stdio.h>
#include <cs50.h>

/*finds number of digits the credit card has and passes
integer to function starting_numbers*/
int card_length(long card_num)
{
    int length = 0; //counter
    long checked_num = card_num;
    while (checked_num > 0)
    {
        checked_num /=  10;
        length++;
    }
    return length;
}

/*checks if the card  has the correct length and starting
numbers */
void starting_numbers(long card_num)
{
    int length = card_length(card_num); //length of card

    //AMEX has a length of 15 and starting numbers of either 34 or 37
    if (length == 15)
    {
        card_num = card_num / 10000000000000;
        if (card_num == 34 || card_num == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    // VISA has a length of 13 and its is starting number 4
    else if (length == 13)
    {
        card_num = card_num / 1000000000000;
        if (card_num == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }

    }
    // Mastercard and VISA can both have a length of 16
    else if (length == 16)
    {
        //checks if the card is VISA by checking if the first number is 4
        long visa_card = card_num / 1000000000000000;
        long mc_card = card_num / 100000000000000;
        if (visa_card == 4)
        {
            printf("VISA\n");
        }
        /*checks if the card is Mastercard by checking if the first numbers
        are 51, 52, 53, 54, or 55*/
        else if (mc_card > 50 && mc_card < 56)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    //if the card doesn't have the correct length, print INVALID
    else
    {
        printf("INVALID\n");
    }
}

//Checks if the card numbers satisfy Luhn's algorithm
bool luhns(long card_num)
{
    int total = 0;
    while (card_num != 0)
    {
        // adds every other digit to the total
        total += card_num % 10;
        card_num = card_num / 10;

        //multiplies every other digit by 2
        int product = (card_num % 10) * 2;

        //if the product has 2 digits, add each individual digit to total
        if (product > 9)
        {
            total += product % 10;
            product = product / 10;
            total += product;
        }
        //if the product has 1 digit, product is added as is
        else
        {
            total = total + product;
        }
        card_num = card_num / 10;
    }

    /*checks whether the total's last digit is 0;
    returns TRUE if yes, FALSE otherwise*/
    if (total % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main(void)
{
    long card_num = get_long("Number: ");

    if (luhns(card_num))
    {
        starting_numbers(card_num);
    }
    else
    {
        printf("INVALID\n");
    }
}