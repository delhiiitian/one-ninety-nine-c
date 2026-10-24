/*
 * Chapter 2.9 - Logical operators and the ternary operator
 * && and, || or, ! not, and condition ? a : b
 */
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    int  age = 17;
    bool with_parent = true;
    bool has_id = false;

    /* && needs both, || needs at least one */
    bool can_watch_r_movie = age >= 18 || with_parent;
    printf("Can watch an R-rated movie: %s\n", can_watch_r_movie ? "yes" : "no");

    bool can_buy_lottery = age >= 18 && has_id;
    printf("Can buy a lottery ticket:   %s\n", can_buy_lottery ? "yes" : "no");

    printf("Has no ID: %s\n", !has_id ? "true" : "false");

    /* leap year rule in one expression */
    for (int year = 1900; year <= 2028; year += 32) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
        printf("%d is %sa leap year\n", year, leap ? "" : "not ");
    }

    /* short-circuit: the right side is skipped when the left decides */
    int items = 0, total = 0;
    if (items != 0 && total / items > 10)   /* no division by zero */
        printf("big average\n");
    else
        printf("Short-circuit avoided dividing by zero.\n");
    return 0;
}
