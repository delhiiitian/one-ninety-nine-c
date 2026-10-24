/*
 * Chapter 5.5 - Checking malloc and cleaning up on failure
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* duplicate a string onto the heap; caller must free the result */
char *copy_text(const char *src)
{
    size_t n = strlen(src) + 1;
    char *dst = malloc(n);
    if (dst == NULL)
        return NULL;                  /* pass the failure up */
    memcpy(dst, src, n);
    return dst;
}

/* allocate rows*cols safely: refuse sizes whose multiplication would overflow */
double *make_grid(size_t rows, size_t cols)
{
    if (cols != 0 && rows > SIZE_MAX / cols / sizeof(double))
        return NULL;
    return calloc(rows * cols, sizeof(double));
}

int main(void)
{
    char *name = copy_text("Priya Sharma");
    if (name == NULL) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }
    printf("Copied: %s\n", name);

    double *grid = make_grid(1000, 1000);
    if (grid == NULL) {
        fprintf(stderr, "could not allocate grid\n");
        free(name);                   /* release what we already have */
        return EXIT_FAILURE;
    }
    printf("Allocated a 1000x1000 grid (%zu MB)\n", 1000 * 1000 * sizeof(double) / 1000000);

    /* a request so large it must fail: we handle it instead of crashing */
    double *huge = make_grid(SIZE_MAX / 2, 4);
    printf("Absurd grid request %s\n", huge == NULL ? "was refused safely" : "succeeded?!");
    free(huge);                       /* free(NULL) is allowed and does nothing */

    free(grid);
    free(name);
    return EXIT_SUCCESS;
}
