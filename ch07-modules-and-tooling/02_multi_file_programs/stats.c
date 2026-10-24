#include "stats.h"

double mean(const double *v, int n)
{
    double s = 0;
    for (int i = 0; i < n; i++)
        s += v[i];
    return n > 0 ? s / n : 0;
}

double max_value(const double *v, int n)
{
    double m = v[0];
    for (int i = 1; i < n; i++)
        if (v[i] > m)
            m = v[i];
    return m;
}
