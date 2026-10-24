/*
 * Chapter 3.5 - static local variables: memory that survives between calls
 */
#include <stdio.h>

/* hands out ticket numbers like a bakery's "take a number" machine */
int next_ticket(void)
{
    static int counter = 100;   /* initialised once, keeps its value between calls */
    counter++;
    return counter;
}

int forgetful_counter(void)
{
    int counter = 100;          /* ordinary local: reset on every call */
    counter++;
    return counter;
}

int main(void)
{
    for (int i = 0; i < 3; i++)
        printf("Static ticket:   %d\n", next_ticket());
    for (int i = 0; i < 3; i++)
        printf("Ordinary ticket: %d\n", forgetful_counter());
    return 0;
}
