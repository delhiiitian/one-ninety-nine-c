/*
 * Chapter 7.1 - Header files
 * Build: gcc -std=c11 -Wall main.c temperature.c -o temps
 */
#include <stdio.h>           /* angle brackets: system headers */
#include "temperature.h"
#include "temperature.h"     /* included twice on purpose: the guard makes it harmless */

int main(void)
{
    double oven_f = 350;
    double c = to_celsius(oven_f, FAHRENHEIT);
    printf("Oven at %.0f %s = %.1f %s = %.1f %s\n", oven_f, scale_symbol(FAHRENHEIT),
           c, scale_symbol(CELSIUS), from_celsius(c, KELVIN), scale_symbol(KELVIN));
    printf("Absolute zero is %.2f C\n", ABSOLUTE_ZERO_C);
    return 0;
}
