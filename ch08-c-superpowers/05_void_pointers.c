/*
 * Chapter 8.5 - Generic code with void pointers
 * void * can point at anything; you pass the element size and a function that knows the type.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* swap any two objects of `size` bytes */
void swap_any(void *a, void *b, size_t size)
{
    unsigned char tmp[64];
    if (size > sizeof tmp) return;
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

/* a generic linear search: returns a pointer to the match or NULL */
void *find_any(const void *base, size_t count, size_t size, const void *key,
               int (*equal)(const void *, const void *))
{
    const unsigned char *p = base;               /* byte pointer so we can step by size */
    for (size_t i = 0; i < count; i++)
        if (equal(p + i * size, key))
            return (void *)(p + i * size);
    return NULL;
}

typedef struct { char code[4]; double rate; } Currency;

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);                    /* avoids overflow of x - y */
}

static int cmp_currency_rate(const void *a, const void *b)
{
    double x = ((const Currency *)a)->rate, y = ((const Currency *)b)->rate;
    return (x > y) - (x < y);
}

static int same_code(const void *element, const void *key)
{
    return strcmp(((const Currency *)element)->code, key) == 0;
}

int main(void)
{
    int a = 1, b = 2;
    double x = 1.5, y = 2.5;
    swap_any(&a, &b, sizeof a);
    swap_any(&x, &y, sizeof x);
    printf("swapped ints: %d %d, doubles: %.1f %.1f\n", a, b, x, y);

    int ages[] = { 42, 7, 19, 88, 3 };
    qsort(ages, 5, sizeof ages[0], cmp_int);     /* the library's generic sort */
    printf("sorted ages:");
    for (int i = 0; i < 5; i++) printf(" %d", ages[i]);
    printf("\n");

    Currency fx[] = { { "EUR", 0.92 }, { "INR", 83.1 }, { "GBP", 0.79 }, { "JPY", 149.5 } };
    qsort(fx, 4, sizeof fx[0], cmp_currency_rate);     /* same qsort, different type */
    for (int i = 0; i < 4; i++) printf("  1 USD = %7.2f %s\n", fx[i].rate, fx[i].code);

    Currency *inr = find_any(fx, 4, sizeof fx[0], "INR", same_code);
    if (inr) printf("found %s at rate %.1f\n", inr->code, inr->rate);

    int key = 19;
    int *hit = bsearch(&key, ages, 5, sizeof ages[0], cmp_int);   /* binary search, sorted data */
    printf("bsearch for 19: %s\n", hit ? "found" : "missing");
    return 0;
}
