/*
 * Chapter 2.5 - for: counting loops
 * for (start; condition; step) keeps all three parts of a counting loop on one line.
 */
#include <stdio.h>

int main(void)
{
    /* countdown */
    for (int i = 5; i > 0; i--) {
        printf("%d... ", i);
    }
    printf("Lift off!\n");

    /* multiplication table of 7 */
    for (int i = 1; i <= 10; i++) {
        printf("7 x %2d = %2d\n", i, 7 * i);
    }

    /* walking through an array */
    int steps[] = { 4200, 8100, 10050, 6300, 12000, 3900, 9000 };
    int days = sizeof steps / sizeof steps[0];   /* number of elements */
    int total = 0;
    for (int d = 0; d < days; d++) {
        total += steps[d];
    }
    printf("Weekly steps: %d, daily average: %d\n", total, total / days);
    return 0;
}
