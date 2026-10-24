/*
 * Chapter 6.7 - Command-line arguments: argc and argv
 * Try: ./command_line_args -n 3 -u hello world
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s [-n COUNT] [-u] WORD...\n"
                    "  -n COUNT  repeat each word COUNT times (default 1)\n"
                    "  -u        print in upper case\n", prog);
}

int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
        printf("argv[%d] = \"%s\"\n", i, argv[i]);

    int repeat = 1, upper = 0, i = 1;
    for (; i < argc && argv[i][0] == '-'; i++) {    /* options come first */
        if (strcmp(argv[i], "-u") == 0) {
            upper = 1;
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            repeat = atoi(argv[++i]);               /* the option's value is the NEXT argument */
            if (repeat < 1) { usage(argv[0]); return 2; }
        } else {
            usage(argv[0]);
            return 2;
        }
    }

    if (i == argc) {                                /* no words: use a default so it still runs */
        static char *defaults[] = { "hello", "from", "argv" };
        printf("(no words given, using defaults)\n");
        for (int d = 0; d < 3; d++)
            printf("%s ", defaults[d]);
        printf("\n");
        return 0;
    }
    for (; i < argc; i++)
        for (int r = 0; r < repeat; r++) {
            for (const char *c = argv[i]; *c; c++)
                putchar(upper ? toupper((unsigned char)*c) : *c);
            putchar(' ');
        }
    putchar('\n');
    return 0;
}
