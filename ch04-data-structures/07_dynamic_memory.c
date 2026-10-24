/*
 * Chapter 4.7 - Dynamic memory: malloc, realloc, free
 * Ask for memory at run time when you do not know the size in advance.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int capacity = 2, count = 0;
    int *readings = malloc(capacity * sizeof *readings);   /* room for 2 ints */
    if (readings == NULL) {
        printf("Out of memory\n");
        return 1;
    }

    /* pretend sensor readings arrive one by one; grow the array as needed */
    int incoming[] = { 12, 15, 11, 19, 22, 18, 25 };
    for (int i = 0; i < 7; i++) {
        if (count == capacity) {
            capacity *= 2;                                  /* double the room */
            int *bigger = realloc(readings, capacity * sizeof *readings);
            if (bigger == NULL) {
                free(readings);
                printf("Out of memory\n");
                return 1;
            }
            readings = bigger;
            printf("Grew storage to %d readings\n", capacity);
        }
        readings[count++] = incoming[i];
    }

    printf("Stored %d readings:", count);
    for (int i = 0; i < count; i++)
        printf(" %d", readings[i]);
    printf("\n");

    free(readings);           /* give the memory back */
    readings = NULL;          /* avoid using it by accident later */

    /* calloc: allocate and zero-fill */
    double *zeros = calloc(5, sizeof *zeros);
    if (zeros != NULL) {
        printf("calloc gives zeros: %.1f %.1f\n", zeros[0], zeros[4]);
        free(zeros);
    }
    return 0;
}
