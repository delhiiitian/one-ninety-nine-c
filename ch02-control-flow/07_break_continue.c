/*
 * Chapter 2.7 - break and continue
 * break leaves the loop now; continue skips to the next round.
 */
#include <stdio.h>

int main(void)
{
    int orders[] = { 12, -1, 30, 0, 45, 999, 18 };   /* -1 and 0 are bad, 999 means "end" */
    int count = sizeof orders / sizeof orders[0];
    int total = 0;

    for (int i = 0; i < count; i++) {
        if (orders[i] == 999) {
            printf("End marker found, stopping.\n");
            break;                   /* exits the for loop completely */
        }
        if (orders[i] <= 0) {
            printf("Skipping bad order %d\n", orders[i]);
            continue;                /* jump to i++ and the next order */
        }
        total += orders[i];
        printf("Added order of %d, running total %d\n", orders[i], total);
    }
    printf("Total of valid orders: %d\n", total);

    /* search: find the first number divisible by 7 and 11 */
    int n;
    for (n = 100; n < 1000; n++) {
        if (n % 7 == 0 && n % 11 == 0)
            break;
    }
    printf("First number >= 100 divisible by 7 and 11: %d\n", n);
    return 0;
}
