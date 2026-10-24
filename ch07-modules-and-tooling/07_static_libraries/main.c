/*
 * Chapter 7.7 - Static libraries
 *   gcc -c number_theory.c primes.c          (compile the library's files)
 *   ar rcs libmathx.a number_theory.o primes.o (bundle them into libmathx.a)
 *   gcc main.c -L. -lmathx -o primes_demo     (link: -L where, -l which library)
 */
#include <stdio.h>
#include "mathx.h"

int main(void)
{
    printf("gcd(84, 36) = %ld, lcm(4, 6) = %ld\n", gcd(84, 36), lcm(4, 6));
    printf("Buses every 12 and 18 minutes meet again after %ld minutes\n", lcm(12, 18));
    printf("Primes below 50:");
    for (long n = 1; n < 50; n++)
        if (is_prime(n))
            printf(" %ld", n);
    printf("\nFirst prime after 1000: %ld\n", next_prime(1000));
    return 0;
}
