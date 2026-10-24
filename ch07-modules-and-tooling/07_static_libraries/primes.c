#include "mathx.h"

int is_prime(long n)
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (long d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return 0;
    return 1;
}

long next_prime(long n)
{
    long p = n + 1;
    while (!is_prime(p))
        p++;
    return p;
}
