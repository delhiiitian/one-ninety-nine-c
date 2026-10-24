/*
 * Chapter 8.8 - Arena allocators: one big block, many quick allocations, one free
 */
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char *base;
    size_t         size;
    size_t         used;
} Arena;

int arena_init(Arena *a, size_t size)
{
    a->base = malloc(size);
    a->size = size;
    a->used = 0;
    return a->base ? 0 : -1;
}

/* hand out the next `size` bytes, aligned for any type; NULL when full */
void *arena_alloc(Arena *a, size_t size)
{
    size_t align = alignof(max_align_t);
    size_t start = (a->used + align - 1) & ~(align - 1);    /* round up to a multiple of align */
    if (start + size > a->size)
        return NULL;
    a->used = start + size;
    return a->base + start;
}

char *arena_strdup(Arena *a, const char *s)
{
    size_t n = strlen(s) + 1;
    char *p = arena_alloc(a, n);
    if (p) memcpy(p, s, n);
    return p;
}

void arena_reset(Arena *a) { a->used = 0; }          /* "free" everything at once */
void arena_free(Arena *a)  { free(a->base); a->base = NULL; }

typedef struct Token { const char *text; struct Token *next; } Token;

/* split a sentence into a linked list of tokens, all memory from the arena */
Token *tokenize(Arena *a, const char *sentence)
{
    Token *head = NULL, **tail = &head;
    char word[64];
    int n = 0;
    for (const char *p = sentence;; p++) {
        if (*p && *p != ' ' && n < 63) {
            word[n++] = *p;
            continue;
        }
        if (n > 0) {
            word[n] = '\0';
            Token *t = arena_alloc(a, sizeof *t);
            if (t == NULL) break;
            t->text = arena_strdup(a, word);
            t->next = NULL;
            *tail = t;
            tail = &t->next;
            n = 0;
        }
        if (*p == '\0') break;
    }
    return head;
}

int main(void)
{
    Arena arena;
    if (arena_init(&arena, 4096) != 0) return 1;

    const char *requests[] = { "GET /index.html HTTP/1.1", "POST /api/orders HTTP/1.1" };
    for (int r = 0; r < 2; r++) {
        Token *tokens = tokenize(&arena, requests[r]);
        printf("request %d:", r + 1);
        for (Token *t = tokens; t; t = t->next)
            printf(" [%s]", t->text);
        printf("  (arena used %zu bytes)\n", arena.used);
        arena_reset(&arena);          /* done with this request: everything freed in O(1) */
    }

    void *too_big = arena_alloc(&arena, 10000);
    printf("asking for 10000 bytes from a 4096-byte arena gives %s\n", too_big ? "memory?!" : "NULL");
    arena_free(&arena);
    return 0;
}
