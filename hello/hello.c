#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //stores name input by user into string and responds
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);
}