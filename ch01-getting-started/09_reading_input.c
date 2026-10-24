/*
 * Chapter 1.9 - Reading input with scanf
 * scanf reads typed text and converts it into variables. Pass the ADDRESS (&) of each variable.
 * Try: echo "Asha 29 1.62" | ./reading_input
 */
#include <stdio.h>

int main(void)
{
    char   name[50];
    int    age;
    double height_m;

    printf("Enter your first name, age and height in metres: ");
    /* %49s stops at 49 chars so the 50-char box cannot overflow */
    int read = scanf("%49s %d %lf", name, &age, &height_m);

    if (read != 3) {                     /* scanf returns how many items it filled */
        printf("\nSorry, I expected a name, a whole number and a decimal.\n");
        return 1;
    }

    printf("\nHello %s!\n", name);
    printf("In 10 years you will be %d.\n", age + 10);
    printf("Your height is %.0f cm.\n", height_m * 100);
    return 0;
}
