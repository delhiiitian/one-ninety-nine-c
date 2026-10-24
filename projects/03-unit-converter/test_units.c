/* test_units.c - Run: make test */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "units.h"

static int close_to(double a, double b) { return fabs(a - b) < 1e-6; }

int main(void)
{
    double r;
    assert(convert(1, find_unit("km"), find_unit("m"), &r) == 0 && close_to(r, 1000));
    assert(convert(1, find_unit("mi"), find_unit("km"), &r) == 0 && close_to(r, 1.609344));
    assert(convert(1, find_unit("lb"), find_unit("kg"), &r) == 0 && close_to(r, 0.45359237));
    assert(convert(100, find_unit("C"), find_unit("F"), &r) == 0 && close_to(r, 212));
    assert(convert(0, find_unit("K"), find_unit("C"), &r) == 0 && close_to(r, -273.15));
    assert(convert(-500, find_unit("C"), find_unit("K"), &r) == -1);   /* below absolute zero */
    assert(convert(1, find_unit("kg"), find_unit("m"), &r) == -1);     /* different categories */
    assert(find_unit("parsec") == NULL);
    printf("All unit tests passed.\n");
    return 0;
}
