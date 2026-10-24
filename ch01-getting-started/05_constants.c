/*
 * Chapter 1.5 - Constants: values that must not change
 * Three tools: #define (text replacement), const (read-only variable), enum (named integers).
 */
#include <stdio.h>

#define SALES_TAX_PERCENT 8          /* preprocessor constant, no type, no memory */

enum Weekday { MON = 1, TUE, WED, THU, FRI, SAT, SUN };   /* TUE = 2, ... */

int main(void)
{
    const double PI = 3.14159265358979;   /* typed, checked by the compiler */
    const int    MAX_SEATS = 180;

    double radius = 2.0;
    printf("Circle area (r=2): %.3f\n", PI * radius * radius);

    int price = 50;
    printf("Price $%d + %d%% tax = $%.2f\n", price, SALES_TAX_PERCENT,
           price * (1 + SALES_TAX_PERCENT / 100.0));

    printf("Seats on the plane: %d\n", MAX_SEATS);
    /* MAX_SEATS = 200;  <- uncomment and the compiler refuses: read-only */

    enum Weekday today = FRI;
    printf("Friday is day %d of the week; weekend? %s\n",
           today, (today >= SAT) ? "yes" : "no");
    return 0;
}
