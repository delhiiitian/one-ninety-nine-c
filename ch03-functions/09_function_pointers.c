/*
 * Chapter 3.9 - Function pointers: passing behaviour as a value
 */
#include <stdio.h>

double add(double a, double b)      { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b)   { return b != 0 ? a / b : 0; }

/* a type name for "pointer to a function taking two doubles, returning a double" */
typedef double (*Operation)(double, double);

/* a function that takes another function as a parameter */
void apply_to_all(const double xs[], int n, double y, Operation op, const char *name)
{
    printf("%-9s:", name);
    for (int i = 0; i < n; i++)
        printf(" %6.2f", op(xs[i], y));     /* call through the pointer */
    printf("\n");
}

int main(void)
{
    /* a table of buttons, like a calculator keypad */
    struct { char symbol; Operation op; } keys[] = {
        { '+', add }, { '-', subtract }, { '*', multiply }, { '/', divide },
    };
    for (int i = 0; i < 4; i++)
        printf("12 %c 4 = %.2f\n", keys[i].symbol, keys[i].op(12, 4));

    double prices[] = { 10, 20, 30 };
    apply_to_all(prices, 3, 2, multiply, "doubled");
    apply_to_all(prices, 3, 5, subtract, "minus 5");
    return 0;
}
