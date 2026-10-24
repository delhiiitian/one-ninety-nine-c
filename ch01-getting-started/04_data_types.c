/*
 * Chapter 1.4 - Data types and their sizes
 * The type decides how many bytes a value uses and what range it can hold.
 */
#include <stdio.h>
#include <limits.h>   /* INT_MAX, CHAR_MAX, ... */
#include <float.h>    /* DBL_MAX, FLT_DIG, ... */
#include <stdbool.h>  /* bool, true, false */

int main(void)
{
    printf("%-14s %5s  %s\n", "type", "bytes", "range / note");
    printf("%-14s %5zu  %d to %d\n", "char", sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("%-14s %5zu  %d to %d\n", "short", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("%-14s %5zu  %d to %d\n", "int", sizeof(int), INT_MIN, INT_MAX);
    printf("%-14s %5zu  %ld to %ld\n", "long", sizeof(long), LONG_MIN, LONG_MAX);
    printf("%-14s %5zu  0 to %u\n", "unsigned int", sizeof(unsigned int), UINT_MAX);
    printf("%-14s %5zu  about %d significant digits\n", "float", sizeof(float), FLT_DIG);
    printf("%-14s %5zu  about %d significant digits\n", "double", sizeof(double), DBL_DIG);
    printf("%-14s %5zu  true=%d false=%d\n", "bool", sizeof(bool), true, false);

    /* Why it matters: float keeps fewer digits than double */
    float  f = 1.0f / 3.0f;
    double d = 1.0 / 3.0;
    printf("\n1/3 as float : %.17f\n", f);
    printf("1/3 as double: %.17f\n", d);
    return 0;
}
