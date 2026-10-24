/* counter.h - counts lines, words, bytes and word frequencies from any FILE* */
#ifndef COUNTER_H
#define COUNTER_H

#include <stdio.h>
#include <stddef.h>

typedef struct {
    long lines;
    long words;
    long bytes;
    long longest_line;
} Counts;

typedef struct WordEntry {
    char             *word;
    long              count;
    struct WordEntry *next;      /* chaining for collisions */
} WordEntry;

typedef struct {
    WordEntry **buckets;
    size_t      bucket_count;
    size_t      unique;
} WordTable;

int    count_stream(FILE *in, Counts *counts, WordTable *table);  /* table may be NULL */
void   counts_add(Counts *total, const Counts *c);

int    table_init(WordTable *t, size_t buckets);
int    table_add(WordTable *t, const char *word);
size_t table_top(const WordTable *t, WordEntry **out, size_t max);
void   table_free(WordTable *t);

#endif
