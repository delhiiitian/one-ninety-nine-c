/*
 * Chapter 4.1 - Arrays: many values of one type, side by side
 */
#include <stdio.h>

#define DAYS 7

int main(void)
{
    /* a week of temperatures; index 0 is Monday */
    double temps[DAYS] = { 21.5, 23.0, 19.8, 25.1, 27.4, 22.0, 18.9 };
    const char *names[DAYS] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

    double sum = 0, hottest = temps[0];
    int hottest_day = 0;
    for (int i = 0; i < DAYS; i++) {
        sum += temps[i];
        if (temps[i] > hottest) {
            hottest = temps[i];
            hottest_day = i;
        }
    }
    printf("Average: %.1f C\n", sum / DAYS);
    printf("Hottest: %s at %.1f C\n", names[hottest_day], hottest);

    int scores[5] = { 0 };       /* every element set to 0 */
    scores[2] = 88;              /* change one element */
    printf("scores:");
    for (int i = 0; i < 5; i++)
        printf(" %d", scores[i]);
    printf("\n");

    printf("The array uses %zu bytes: %d elements of %zu bytes\n",
           sizeof temps, DAYS, sizeof temps[0]);
    /* temps[7] would be OUT OF BOUNDS: C does not check, it just reads garbage */
    return 0;
}
