/*
 * Chapter 7.2 - Programs made of several files
 * Compile each file separately, then link:
 *   gcc -c geometry.c        -> geometry.o
 *   gcc -c stats.c           -> stats.o
 *   gcc -c main.c            -> main.o
 *   gcc main.o geometry.o stats.o -lm -o fields
 */
#include <stdio.h>
#include "geometry.h"
#include "stats.h"

int main(void)
{
    /* three farm fields, corners in metres */
    Point square[]   = { { 0, 0 }, { 100, 0 }, { 100, 100 }, { 0, 100 } };
    Point triangle[] = { { 0, 0 }, { 120, 0 }, { 0, 50 } };
    Point lshape[]   = { { 0, 0 }, { 80, 0 }, { 80, 30 }, { 30, 30 }, { 30, 90 }, { 0, 90 } };

    double areas[3] = {
        polygon_area(square, 4), polygon_area(triangle, 3), polygon_area(lshape, 6),
    };
    printf("Square:   area %7.1f m2, fence %6.1f m\n", areas[0], polygon_perimeter(square, 4));
    printf("Triangle: area %7.1f m2, fence %6.1f m\n", areas[1], polygon_perimeter(triangle, 3));
    printf("L-shape:  area %7.1f m2, fence %6.1f m\n", areas[2], polygon_perimeter(lshape, 6));
    printf("Average field %.1f m2, largest %.1f m2\n", mean(areas, 3), max_value(areas, 3));
    return 0;
}
