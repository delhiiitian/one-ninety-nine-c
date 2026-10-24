/*
 * Chapter 3.6 - Recursion: a function that calls itself
 * Every recursive function needs a base case (stop) and a smaller step.
 */
#include <stdio.h>

unsigned long long factorial(unsigned int n)
{
    if (n <= 1)                     /* base case */
        return 1;
    return n * factorial(n - 1);    /* smaller problem */
}

/* folders inside folders: print an indented tree to a given depth */
void print_folder(const char *name, int depth, int max_depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("%s/\n", name);
    if (depth == max_depth)         /* base case */
        return;
    print_folder("photos", depth + 1, max_depth);
    print_folder("docs", depth + 1, max_depth);
}

/* sum of digits: 1234 -> 1 + 2 + 3 + 4 */
int digit_sum(int n)
{
    if (n < 10)
        return n;
    return n % 10 + digit_sum(n / 10);
}

int main(void)
{
    for (unsigned int n = 0; n <= 10; n += 5)
        printf("%u! = %llu\n", n, factorial(n));
    printf("20! = %llu\n", factorial(20));
    printf("digit sum of 98765 = %d\n", digit_sum(98765));
    print_folder("home", 0, 2);
    return 0;
}
