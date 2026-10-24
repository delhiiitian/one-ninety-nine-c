/*
 * Chapter 7.8 - Debugging with GDB
 * Build with symbols and no optimisation:  gcc -g -O0 08_debugging_with_gdb.c -o debug_me
 * Then:   gdb ./debug_me
 *   (gdb) break average_of_top        stop when this function is called
 *   (gdb) run                          start the program
 *   (gdb) next                         run one line
 *   (gdb) print sorted[i]              look at a variable
 *   (gdb) info locals                  show all local variables
 *   (gdb) backtrace                    who called whom
 *   (gdb) continue                     run to the next breakpoint
 *   (gdb) quit
 * Run "./debug_me --bug" to use the buggy version that crashes or prints nonsense.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare_desc(const void *a, const void *b)
{
    return *(const int *)b - *(const int *)a;
}

/* average of the best k scores (k must be <= n) */
double average_of_top(const int *scores, int n, int k, int buggy)
{
    int *sorted = malloc((n + 1) * sizeof *sorted);   /* one spare slot keeps the bug from crashing */
    if (sorted == NULL)
        return 0;
    memcpy(sorted, scores, n * sizeof *sorted);
    sorted[n] = 0;
    qsort(sorted, n, sizeof *sorted, compare_desc);

    int sum = 0;
    for (int i = 0; buggy ? i <= k : i < k; i++)      /* the bug: <= adds one score too many */
        sum += sorted[i];
    free(sorted);
    return (double)sum / k;
}

int main(int argc, char *argv[])
{
    int buggy = argc > 1 && strcmp(argv[1], "--bug") == 0;
    int scores[] = { 72, 95, 88, 61, 79, 90 };
    double avg = average_of_top(scores, 6, 3, buggy);
    printf("Average of top 3: %.2f %s\n", avg, buggy ? "(wrong! use gdb to find out why)" : "");
    return 0;
}
