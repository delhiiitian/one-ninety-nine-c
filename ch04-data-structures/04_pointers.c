/*
 * Chapter 4.4 - Pointers: variables that hold addresses
 */
#include <stdio.h>

int main(void)
{
    int stock = 40;
    int *p = &stock;              /* p holds the address of stock */

    printf("stock      = %d\n", stock);
    printf("&stock     = %p (where it lives)\n", (void *)&stock);
    printf("p          = %p (same address)\n", (void *)p);
    printf("*p         = %d (the value at that address)\n", *p);

    *p = 35;                      /* change stock THROUGH the pointer */
    printf("after *p = 35, stock = %d\n", stock);

    int other = 100;
    p = &other;                   /* a pointer can be re-aimed */
    printf("p now points at other: *p = %d\n", *p);

    int *nothing = NULL;          /* NULL: points at nothing; always check before using */
    if (nothing == NULL)
        printf("nothing is NULL, so we do not dereference it\n");

    /* pointer to a pointer-sized idea: sizes of pointers are all the same */
    double price = 9.99;
    double *pp = &price;
    printf("sizeof(int*) = %zu, sizeof(double*) = %zu\n", sizeof p, sizeof pp);
    return 0;
}
