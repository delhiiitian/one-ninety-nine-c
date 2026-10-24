#include "mathx.h"

long gcd(long a, long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {          /* Euclid's algorithm */
        long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

long lcm(long a, long b)
{
    return (a == 0 || b == 0) ? 0 : a / gcd(a, b) * b;
}
