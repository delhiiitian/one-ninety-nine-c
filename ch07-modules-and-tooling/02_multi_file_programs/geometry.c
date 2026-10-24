#include <math.h>
#include "geometry.h"

double distance(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double polygon_perimeter(const Point *pts, int n)
{
    double total = 0;
    for (int i = 0; i < n; i++)
        total += distance(pts[i], pts[(i + 1) % n]);   /* last point joins the first */
    return total;
}

/* shoelace formula */
double polygon_area(const Point *pts, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++) {
        Point a = pts[i], b = pts[(i + 1) % n];
        sum += a.x * b.y - b.x * a.y;
    }
    return fabs(sum) / 2.0;
}
