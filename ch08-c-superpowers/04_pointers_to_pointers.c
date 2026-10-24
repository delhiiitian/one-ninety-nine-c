/*
 * Chapter 8.4 - Pointers to pointers
 * Used for arrays of strings, for functions that change a caller's pointer,
 * and for 2D data built at run time.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* a function that must change the CALLER's pointer takes char ** */
int read_name(const char *source, char **out)
{
    *out = malloc(strlen(source) + 1);
    if (*out == NULL)
        return -1;
    strcpy(*out, source);
    return 0;
}

/* build a ragged 2D array: row i has i + 1 entries (Pascal's triangle) */
int **pascal(int rows)
{
    int **t = malloc(rows * sizeof *t);       /* array of row pointers */
    if (t == NULL) return NULL;
    for (int r = 0; r < rows; r++) {
        t[r] = malloc((r + 1) * sizeof **t);  /* each row separately */
        if (t[r] == NULL) {
            while (r-- > 0) free(t[r]);
            free(t);
            return NULL;
        }
        t[r][0] = t[r][r] = 1;
        for (int c = 1; c < r; c++)
            t[r][c] = t[r - 1][c - 1] + t[r - 1][c];
    }
    return t;
}

int main(void)
{
    /* argv-style: an array of pointers to strings */
    const char *planets[] = { "Mercury", "Venus", "Earth", "Mars", NULL };
    for (const char **p = planets; *p != NULL; p++)
        printf("%s (%zu letters)\n", *p, strlen(*p));

    char *name = NULL;
    if (read_name("Ada Lovelace", &name) == 0) {   /* pass the ADDRESS of our pointer */
        printf("read_name set our pointer to \"%s\"\n", name);
        free(name);
    }

    int rows = 6;
    int **tri = pascal(rows);
    if (tri == NULL) return 1;
    for (int r = 0; r < rows; r++) {
        printf("%*s", (rows - r) * 2, "");
        for (int c = 0; c <= r; c++)
            printf("%4d", tri[r][c]);
        printf("\n");
        free(tri[r]);
    }
    free(tri);
    return 0;
}
