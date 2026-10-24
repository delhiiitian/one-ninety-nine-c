/*
 * Chapter 3.7 - Pass by value vs pass by pointer
 * C copies arguments. To let a function change your variable, pass its address.
 */
#include <stdio.h>

void try_to_double(int x)          /* gets a copy */
{
    x = x * 2;
}

void really_double(int *x)         /* gets the address */
{
    *x = *x * 2;                   /* *x means "the variable x points at" */
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* "return" two results by writing through pointers */
void split_minutes(int total_minutes, int *hours, int *minutes)
{
    *hours = total_minutes / 60;
    *minutes = total_minutes % 60;
}

int main(void)
{
    int points = 21;
    try_to_double(points);
    printf("After try_to_double: %d (unchanged)\n", points);
    really_double(&points);
    printf("After really_double: %d\n", points);

    int left = 1, right = 2;
    swap(&left, &right);
    printf("After swap: left=%d right=%d\n", left, right);

    int h, m;
    split_minutes(135, &h, &m);
    printf("135 minutes = %d h %d min\n", h, m);
    return 0;
}
