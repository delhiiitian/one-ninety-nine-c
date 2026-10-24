/*
 * temperature.h - the "menu": what this module offers.
 * Any .c file that #includes this can call these functions.
 */
#ifndef TEMPERATURE_H        /* include guard: skip if already included */
#define TEMPERATURE_H

#define ABSOLUTE_ZERO_C (-273.15)

typedef enum { CELSIUS, FAHRENHEIT, KELVIN } Scale;

double to_celsius(double value, Scale from);
double from_celsius(double celsius, Scale to);
const char *scale_symbol(Scale s);

#endif /* TEMPERATURE_H */
