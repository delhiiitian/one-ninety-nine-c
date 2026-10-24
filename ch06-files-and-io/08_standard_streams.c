/*
 * Chapter 6.8 - stdin, stdout and stderr
 * Normal output -> stdout, problems -> stderr, input <- stdin. The shell can redirect each.
 * Try: printf '3\nx\n7\n' | ./standard_streams > sum.txt 2> errors.txt
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char line[64];
    long sum = 0;
    int line_no = 0, good = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {   /* read until end of input */
        line_no++;
        char *end;
        long v = strtol(line, &end, 10);
        if (end == line) {
            fprintf(stderr, "line %d: not a number, ignored\n", line_no);   /* to stderr */
            continue;
        }
        sum += v;
        good++;
    }
    fprintf(stdout, "%d numbers, sum %ld\n", good, sum);  /* same as printf */
    return 0;
}
