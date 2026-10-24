/*
 * Chapter 3.8 - Passing arrays to functions
 * An array argument becomes a pointer to its first element, so pass the length too.
 */
#include <stdio.h>

double average(const int values[], int count)   /* const: we promise not to change them */
{
    if (count == 0)
        return 0.0;
    long sum = 0;
    for (int i = 0; i < count; i++)
        sum += values[i];
    return (double)sum / count;
}

int index_of_max(const int values[], int count)
{
    int best = 0;
    for (int i = 1; i < count; i++)
        if (values[i] > values[best])
            best = i;
    return best;
}

void apply_discount(int prices[], int count, int percent)   /* changes the caller's array */
{
    for (int i = 0; i < count; i++)
        prices[i] -= prices[i] * percent / 100;
}

int main(void)
{
    int sales[] = { 120, 340, 90, 410, 275 };
    int n = sizeof sales / sizeof sales[0];

    printf("Average sale: %.1f\n", average(sales, n));
    printf("Best day: day %d with %d\n", index_of_max(sales, n) + 1, sales[index_of_max(sales, n)]);

    apply_discount(sales, n, 10);
    printf("After 10%% discount:");
    for (int i = 0; i < n; i++)
        printf(" %d", sales[i]);
    printf("\n");
    return 0;
}
