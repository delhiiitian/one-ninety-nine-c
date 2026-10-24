/*
 * Chapter 1.3 - Variables
 * A variable is a named box in memory with a fixed type.
 */
#include <stdio.h>

int main(void)
{
    int    apples = 12;        /* declare and initialise in one line */
    double price_per_apple = 0.25;
    char   grade = 'A';

    int total_cents;           /* declared now, assigned later */
    total_cents = apples * 25;

    printf("Apples: %d\n", apples);
    printf("Price each: $%.2f\n", price_per_apple);
    printf("Total: %d cents ($%.2f)\n", total_cents, apples * price_per_apple);
    printf("Shop rating: %c\n", grade);

    apples = apples - 5;       /* variables can change: we sold 5 */
    printf("Apples left after selling 5: %d\n", apples);

    /* & gives the memory address where the box lives */
    printf("'apples' lives at address %p\n", (void *)&apples);
    return 0;
}
