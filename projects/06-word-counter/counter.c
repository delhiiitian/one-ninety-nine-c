/* counter.c - one pass over the input, character by character */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "counter.h"

#define MAX_WORD 64

/* FNV-1a: a simple, well-known string hash */
static size_t hash(const char *s)
{
    size_t h = 2166136261u;
    for (; *s; s++) {
        h ^= (unsigned char)*s;
        h *= 16777619u;
    }
    return h;
}

int table_init(WordTable *t, size_t buckets)
{
    t->buckets = calloc(buckets, sizeof *t->buckets);
    t->bucket_count = buckets;
    t->unique = 0;
    return t->buckets == NULL ? -1 : 0;
}

int table_add(WordTable *t, const char *word)
{
    size_t i = hash(word) % t->bucket_count;
    for (WordEntry *e = t->buckets[i]; e != NULL; e = e->next)
        if (strcmp(e->word, word) == 0) {
            e->count++;
            return 0;
        }
    WordEntry *e = malloc(sizeof *e);
    if (e == NULL)
        return -1;
    e->word = malloc(strlen(word) + 1);
    if (e->word == NULL) {
        free(e);
        return -1;
    }
    strcpy(e->word, word);
    e->count = 1;
    e->next = t->buckets[i];           /* push onto the front of the chain */
    t->buckets[i] = e;
    t->unique++;
    return 0;
}

static int by_count_desc(const void *a, const void *b)
{
    const WordEntry *x = *(WordEntry *const *)a, *y = *(WordEntry *const *)b;
    if (x->count != y->count)
        return x->count < y->count ? 1 : -1;
    return strcmp(x->word, y->word);   /* ties: alphabetical */
}

/* fills out[] with the most frequent words; returns how many */
size_t table_top(const WordTable *t, WordEntry **out, size_t max)
{
    WordEntry **all = malloc((t->unique ? t->unique : 1) * sizeof *all);
    if (all == NULL)
        return 0;
    size_t n = 0;
    for (size_t i = 0; i < t->bucket_count; i++)
        for (WordEntry *e = t->buckets[i]; e != NULL; e = e->next)
            all[n++] = e;
    qsort(all, n, sizeof *all, by_count_desc);
    size_t k = n < max ? n : max;
    memcpy(out, all, k * sizeof *out);
    free(all);
    return k;
}

void table_free(WordTable *t)
{
    for (size_t i = 0; i < t->bucket_count; i++) {
        WordEntry *e = t->buckets[i];
        while (e != NULL) {
            WordEntry *next = e->next;
            free(e->word);
            free(e);
            e = next;
        }
    }
    free(t->buckets);
    t->buckets = NULL;
    t->unique = 0;
}

int count_stream(FILE *in, Counts *c, WordTable *table)
{
    char word[MAX_WORD];
    size_t word_len = 0;
    int in_word = 0, ch;
    long line_len = 0;
    memset(c, 0, sizeof *c);

    while ((ch = getc(in)) != EOF) {
        c->bytes++;
        if (ch == '\n') {
            c->lines++;
            if (line_len > c->longest_line)
                c->longest_line = line_len;
            line_len = 0;
        } else {
            line_len++;
        }

        if (isspace(ch)) {
            in_word = 0;                          /* a word ends at whitespace (like wc) */
        } else if (!in_word) {
            in_word = 1;
            c->words++;
        }

        /* for frequencies, a "word" is a run of letters, digits or apostrophes, lower-cased */
        if (table != NULL) {
            if (isalnum(ch) || ch == '\'') {
                if (word_len < MAX_WORD - 1)
                    word[word_len++] = (char)tolower(ch);
            } else if (word_len > 0) {
                word[word_len] = '\0';
                if (table_add(table, word) != 0)
                    return -1;
                word_len = 0;
            }
        }
    }
    if (line_len > c->longest_line)
        c->longest_line = line_len;               /* last line without a newline */
    if (table != NULL && word_len > 0) {
        word[word_len] = '\0';
        if (table_add(table, word) != 0)
            return -1;
    }
    return ferror(in) ? -1 : 0;
}

void counts_add(Counts *total, const Counts *c)
{
    total->lines += c->lines;
    total->words += c->words;
    total->bytes += c->bytes;
    if (c->longest_line > total->longest_line)
        total->longest_line = c->longest_line;
}
