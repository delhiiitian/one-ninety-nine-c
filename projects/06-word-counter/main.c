/*
 * Project 6 - Word Counter (a small "wc" with word frequencies)
 * usage: wcount [-l] [-w] [-c] [-L] [-t N] [FILE...]
 *   -l lines  -w words  -c bytes  -L longest line  -t N  top N words
 *   no FILE, or "-", reads standard input
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counter.h"

typedef struct { int lines, words, bytes, longest; long top; } Options;

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s [-l] [-w] [-c] [-L] [-t N] [FILE...]\n", prog);
}

static void print_counts(const Options *o, const Counts *c, const char *name)
{
    if (o->lines)   printf("%8ld", c->lines);
    if (o->words)   printf("%8ld", c->words);
    if (o->bytes)   printf("%8ld", c->bytes);
    if (o->longest) printf("%8ld", c->longest_line);
    printf("  %s\n", name);
}

int main(int argc, char *argv[])
{
    Options o = { 0, 0, 0, 0, 0 };
    int i = 1;
    for (; i < argc && argv[i][0] == '-' && argv[i][1] != '\0'; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            char *end;
            o.top = strtol(argv[++i], &end, 10);
            if (*end != '\0' || o.top <= 0) { usage(argv[0]); return 2; }
            continue;
        }
        for (const char *f = argv[i] + 1; *f; f++) {      /* allow combined flags like -lw */
            switch (*f) {
            case 'l': o.lines = 1; break;
            case 'w': o.words = 1; break;
            case 'c': o.bytes = 1; break;
            case 'L': o.longest = 1; break;
            default:  usage(argv[0]); return 2;
            }
        }
    }
    if (!o.lines && !o.words && !o.bytes && !o.longest)
        o.lines = o.words = o.bytes = 1;                   /* wc's default */

    WordTable table;
    if (o.top > 0 && table_init(&table, 4096) != 0) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    Counts total = { 0, 0, 0, 0 };
    int files = 0, status = 0;
    const char *stdin_only[] = { "-" };
    const char **names = (const char **)&argv[i];
    int name_count = argc - i;
    if (name_count == 0) {
        names = stdin_only;
        name_count = 1;
    }

    for (int k = 0; k < name_count; k++) {
        const char *name = names[k];
        FILE *in = strcmp(name, "-") == 0 ? stdin : fopen(name, "r");
        if (in == NULL) {
            perror(name);                                  /* report, keep going */
            status = 1;
            continue;
        }
        Counts c;
        if (count_stream(in, &c, o.top > 0 ? &table : NULL) != 0) {
            fprintf(stderr, "%s: read error\n", name);
            status = 1;
        }
        if (in != stdin)
            fclose(in);
        print_counts(&o, &c, strcmp(name, "-") == 0 ? "(stdin)" : name);
        counts_add(&total, &c);
        files++;
    }
    if (files > 1)
        print_counts(&o, &total, "total");

    if (o.top > 0) {
        WordEntry **top = malloc((size_t)o.top * sizeof *top);
        if (top != NULL) {
            size_t n = table_top(&table, top, (size_t)o.top);
            printf("\nTop %zu of %zu different words:\n", n, table.unique);
            for (size_t k = 0; k < n; k++)
                printf("%4zu. %-16s %ld\n", k + 1, top[k]->word, top[k]->count);
            free(top);
        }
        table_free(&table);
    }
    return status;
}
