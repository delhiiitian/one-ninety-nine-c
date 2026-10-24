#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct { double x, y; } Point;

double distance(Point a, Point b);
double polygon_perimeter(const Point *pts, int n);
double polygon_area(const Point *pts, int n);

#endif
