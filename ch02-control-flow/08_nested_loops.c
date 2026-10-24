/*
 * Chapter 2.8 - Nested loops: loops inside loops
 * Rows and columns, like seats in a cinema.
 */
#include <stdio.h>

int main(void)
{
    const int rows = 5, seats = 8;
    printf("Cinema map (X = booked):\n   ");
    for (int s = 1; s <= seats; s++)
        printf("%d ", s);
    printf("\n");

    for (int r = 0; r < rows; r++) {             /* outer loop: each row */
        printf("%c  ", 'A' + r);
        for (int s = 1; s <= seats; s++) {       /* inner loop: each seat in the row */
            int booked = (r * seats + s) % 3 == 0;   /* fake booking pattern */
            printf("%c ", booked ? 'X' : '.');
        }
        printf("\n");
    }

    /* a right-angled triangle of stars: inner loop depends on outer */
    for (int line = 1; line <= 4; line++) {
        for (int star = 0; star < line; star++)
            printf("*");
        printf("\n");
    }
    return 0;
}
