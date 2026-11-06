/*
 * Chapter 9.9 - C functions that Python will call through ctypes
 * Build: gcc -O2 -fPIC -shared fastmath.c -o libfastmath.so
 */
#include <stddef.h>
#include <stdint.h>

/* count primes below n with a sieve: a loop Python runs slowly and C runs fast */
long count_primes(long n)
{
    if (n < 3) return 0;
    static unsigned char composite[50000001];
    if (n > 50000000) return -1;
    long count = 0;
    for (long i = 2; i < n; i++) composite[i] = 0;
    for (long i = 2; i < n; i++) {
        if (composite[i]) continue;
        count++;
        for (long j = i * i; j < n; j += i)
            composite[j] = 1;
    }
    return count;
}

/* work on a Python-provided array of doubles */
double mean(const double *values, size_t n)
{
    double s = 0;
    for (size_t i = 0; i < n; i++) s += values[i];
    return n ? s / n : 0;
}

/* fill a caller's buffer: Python passes a writable bytes buffer */
size_t to_upper_copy(const char *src, char *dest, size_t dest_size)
{
    size_t i = 0;
    for (; src[i] && i + 1 < dest_size; i++)
        dest[i] = (src[i] >= 'a' && src[i] <= 'z') ? (char)(src[i] - 32) : src[i];
    dest[i] = '\0';
    return i;
}
