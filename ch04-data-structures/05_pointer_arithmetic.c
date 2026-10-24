/*
 * Chapter 4.5 - Pointer arithmetic: arrays and pointers are close cousins
 */
#include <stdio.h>

int main(void)
{
    int floors[] = { 0, 10, 20, 30, 40 };
    int *p = floors;              /* an array name becomes a pointer to element 0 */

    printf("*p       = %d\n", *p);
    printf("*(p + 2) = %d  (same as floors[2])\n", *(p + 2));

    /* p + 1 moves by one ELEMENT (4 bytes for int), not one byte */
    printf("p     = %p\n", (void *)p);
    printf("p + 1 = %p\n", (void *)(p + 1));

    /* walk the array with a moving pointer */
    printf("walking:");
    for (int *q = floors; q < floors + 5; q++)
        printf(" %d", *q);
    printf("\n");

    /* the difference between two pointers is a count of elements */
    int *first = &floors[0], *last = &floors[4];
    printf("last - first = %td elements apart\n", last - first);

    /* strings: count characters by moving a pointer to the '\0' */
    const char *word = "pointer";
    const char *end = word;
    while (*end != '\0')
        end++;
    printf("'%s' has %td characters\n", word, end - word);
    return 0;
}
