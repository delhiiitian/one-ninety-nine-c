/* units.h - a table-driven unit converter */
#ifndef UNITS_H
#define UNITS_H

typedef enum { LENGTH, WEIGHT, TEMPERATURE, VOLUME, CATEGORY_COUNT } Category;

typedef struct {
    const char *name;        /* "km" */
    const char *long_name;   /* "kilometre" */
    Category    category;
    double      to_base;     /* multiply to get the base unit (metre, gram, litre) */
} Unit;

const Unit *find_unit(const char *name);
const char *category_name(Category c);
int         convert(double value, const Unit *from, const Unit *to, double *result);
void        list_units(void);

#endif
