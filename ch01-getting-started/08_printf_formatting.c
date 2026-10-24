/*
 * Chapter 1.8 - Formatting output with printf
 * Format specifiers: %d int, %f double, %c char, %s text, %x hex, %e scientific.
 * Width and precision: %8.2f = at least 8 wide, 2 decimals. '-' left-aligns.
 */
#include <stdio.h>

int main(void)
{
    const char *items[]  = { "Coffee", "Bagel", "Orange juice" };
    double      prices[] = { 3.5, 2.25, 4.0 };
    int         qty[]    = { 2, 1, 3 };
    double      total = 0;

    printf("%-14s %4s %8s\n", "Item", "Qty", "Amount");
    printf("------------------------------\n");
    for (int i = 0; i < 3; i++) {
        double line = prices[i] * qty[i];
        total += line;
        printf("%-14s %4d %8.2f\n", items[i], qty[i], line);
    }
    printf("------------------------------\n");
    printf("%-14s %13.2f\n", "TOTAL", total);

    printf("\nOther formats:\n");
    printf("hex of 255: %x, octal: %o\n", 255, 255);
    printf("scientific: %e\n", 123456.789);
    printf("zero-padded order id: %06d\n", 42);
    printf("percent sign: 50%%\n");
    return 0;
}
