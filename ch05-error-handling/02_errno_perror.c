/*
 * Chapter 5.2 - errno, perror and strerror
 * Library functions report WHY they failed by setting the global errno.
 */
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    /* 1. opening a file that does not exist */
    FILE *f = fopen("no_such_file.txt", "r");
    if (f == NULL) {
        perror("fopen no_such_file.txt");     /* prints our text + the reason */
        printf("errno = %d, strerror says: %s\n", errno, strerror(errno));
    } else {
        fclose(f);
    }

    /* 2. a number too big for a long */
    errno = 0;                                 /* reset before the call */
    long big = strtol("99999999999999999999999", NULL, 10);
    if (errno == ERANGE)
        printf("strtol: %s (it returned the max, %ld)\n", strerror(errno), big);

    /* 3. maths outside its domain */
    errno = 0;
    double r = sqrt(-1.0);
    if (errno == EDOM)
        printf("sqrt(-1): %s\n", strerror(errno));
    else
        printf("sqrt(-1) returned %f (NaN)\n", r);

    /* errno is only meaningful right after a call that FAILED */
    return 0;
}
