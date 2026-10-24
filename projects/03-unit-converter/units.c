/* units.c - the conversion data and logic */
#include <stdio.h>
#include <string.h>
#include "units.h"

/* one row per unit; adding a unit means adding one line */
static const Unit UNITS[] = {
    { "mm",  "millimetre", LENGTH, 0.001 },
    { "cm",  "centimetre", LENGTH, 0.01 },
    { "m",   "metre",      LENGTH, 1.0 },
    { "km",  "kilometre",  LENGTH, 1000.0 },
    { "in",  "inch",       LENGTH, 0.0254 },
    { "ft",  "foot",       LENGTH, 0.3048 },
    { "mi",  "mile",       LENGTH, 1609.344 },
    { "g",   "gram",       WEIGHT, 1.0 },
    { "kg",  "kilogram",   WEIGHT, 1000.0 },
    { "oz",  "ounce",      WEIGHT, 28.349523125 },
    { "lb",  "pound",      WEIGHT, 453.59237 },
    { "ml",  "millilitre", VOLUME, 0.001 },
    { "l",   "litre",      VOLUME, 1.0 },
    { "gal", "US gallon",  VOLUME, 3.785411784 },
    { "C",   "Celsius",    TEMPERATURE, 0 },
    { "F",   "Fahrenheit", TEMPERATURE, 0 },
    { "K",   "Kelvin",     TEMPERATURE, 0 },
};
static const int UNIT_COUNT = sizeof UNITS / sizeof UNITS[0];

const Unit *find_unit(const char *name)
{
    for (int i = 0; i < UNIT_COUNT; i++)
        if (strcmp(UNITS[i].name, name) == 0)
            return &UNITS[i];
    return NULL;                         /* not found */
}

const char *category_name(Category c)
{
    switch (c) {
    case LENGTH:      return "length";
    case WEIGHT:      return "weight";
    case TEMPERATURE: return "temperature";
    case VOLUME:      return "volume";
    default:          return "unknown";
    }
}

/* temperatures are not simple multiples, so they go through Celsius */
static double to_celsius(double v, char unit)
{
    if (unit == 'F') return (v - 32.0) * 5.0 / 9.0;
    if (unit == 'K') return v - 273.15;
    return v;
}

static double from_celsius(double c, char unit)
{
    if (unit == 'F') return c * 9.0 / 5.0 + 32.0;
    if (unit == 'K') return c + 273.15;
    return c;
}

/* returns 0 on success, -1 if the units cannot be converted */
int convert(double value, const Unit *from, const Unit *to, double *result)
{
    if (from == NULL || to == NULL || from->category != to->category)
        return -1;
    if (from->category == TEMPERATURE) {
        double c = to_celsius(value, from->name[0]);
        if (c < -273.15)
            return -1;                   /* colder than absolute zero */
        *result = from_celsius(c, to->name[0]);
    } else {
        *result = value * from->to_base / to->to_base;
    }
    return 0;
}

void list_units(void)
{
    for (int c = 0; c < CATEGORY_COUNT; c++) {
        printf("  %-12s", category_name((Category)c));
        for (int i = 0; i < UNIT_COUNT; i++)
            if (UNITS[i].category == (Category)c)
                printf(" %s", UNITS[i].name);
        printf("\n");
    }
}
