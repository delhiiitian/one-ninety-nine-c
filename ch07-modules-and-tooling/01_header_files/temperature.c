/* temperature.c - the "kitchen": how the functions really work */
#include "temperature.h"     /* quotes: look in this folder first */

double to_celsius(double value, Scale from)
{
    switch (from) {
    case FAHRENHEIT: return (value - 32.0) * 5.0 / 9.0;
    case KELVIN:     return value + ABSOLUTE_ZERO_C;
    default:         return value;
    }
}

double from_celsius(double c, Scale to)
{
    switch (to) {
    case FAHRENHEIT: return c * 9.0 / 5.0 + 32.0;
    case KELVIN:     return c - ABSOLUTE_ZERO_C;
    default:         return c;
    }
}

const char *scale_symbol(Scale s)
{
    return s == FAHRENHEIT ? "F" : s == KELVIN ? "K" : "C";
}
