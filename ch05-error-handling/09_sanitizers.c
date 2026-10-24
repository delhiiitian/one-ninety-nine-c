/*
 * Chapter 5.9 - Finding memory bugs with sanitizers
 * Build with:  gcc -g -fsanitize=address,undefined 09_sanitizers.c -o sanitizers
 * Then run:    ./sanitizers overflow   (writes past an array)
 *              ./sanitizers leak       (forgets to free)
 *              ./sanitizers uaf        (uses memory after free)
 *              ./sanitizers overflow-int (signed integer overflow)
 * With no argument it runs the CORRECT versions and exits cleanly.
 */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void overflow(int buggy)
{
    int *scores = malloc(5 * sizeof *scores);
    if (scores == NULL) return;
    int n = buggy ? 6 : 5;                   /* bug: one too many */
    for (int i = 0; i < n; i++)
        scores[i] = i * 10;
    printf("overflow test wrote %d scores\n", n);
    free(scores);
}

static void leak(int buggy)
{
    char *note = malloc(64);
    if (note == NULL) return;
    strcpy(note, "remember to free me");
    printf("leak test: %s\n", note);
    if (!buggy)
        free(note);                          /* bug: skipped when buggy */
}

static void use_after_free(int buggy)
{
    int *value = malloc(sizeof *value);
    if (value == NULL) return;
    *value = 7;
    if (buggy) {
        int *volatile alias = value;   /* volatile hides the bug from the compiler's own warning */
        free(value);
        printf("use-after-free reads %d\n", *alias);   /* bug */
    } else {
        printf("use-after-free test reads %d safely\n", *value);
        free(value);
    }
}

static void int_overflow(int buggy)
{
    int big = INT_MAX - (buggy ? 0 : 1);
    big = big + 1;                           /* overflows only when buggy */
    printf("int test: %d\n", big);
}

int main(int argc, char *argv[])
{
    const char *which = argc > 1 ? argv[1] : "";
    overflow(strcmp(which, "overflow") == 0);
    leak(strcmp(which, "leak") == 0);
    use_after_free(strcmp(which, "uaf") == 0);
    int_overflow(strcmp(which, "overflow-int") == 0);
    printf("Done. Rebuild with -fsanitize=address,undefined and pass a bug name.\n");
    return 0;
}
