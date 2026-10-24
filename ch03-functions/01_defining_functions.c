/*
 * Chapter 3.1 - Defining and calling functions
 * A function is a named block of code you can run (call) whenever you need it.
 */
#include <stdio.h>

/* a function that takes nothing and returns nothing */
void print_divider(void)
{
    printf("----------------------------\n");
}

/* a function that takes a value and returns a value */
double celsius_to_fahrenheit(double c)
{
    return c * 9.0 / 5.0 + 32.0;
}

int main(void)
{
    print_divider();
    printf(" Weekly weather report\n");
    print_divider();

    double temps[] = { 18.5, 21.0, 25.3, 30.1, 16.0 };
    const char *days[] = { "Mon", "Tue", "Wed", "Thu", "Fri" };
    for (int i = 0; i < 5; i++) {
        double f = celsius_to_fahrenheit(temps[i]);   /* call it, keep the result */
        printf(" %s  %5.1f C  = %5.1f F\n", days[i], temps[i], f);
    }
    print_divider();                                  /* reuse: write once, call many times */
    return 0;
}
