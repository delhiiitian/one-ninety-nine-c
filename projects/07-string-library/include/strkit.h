/*
 * strkit.h - a small, tested string library for C
 *
 * Two kinds of functions:
 *   - in-place helpers that work on a char buffer you own (str_trim, str_to_upper, ...)
 *   - functions that return NEW heap strings, which the caller must free()
 *   - StrBuilder: a growable string for building text piece by piece
 */
#ifndef STRKIT_H
#define STRKIT_H

#include <stdbool.h>
#include <stddef.h>

#define STRKIT_VERSION "1.0.0"

/* ---- questions about strings (never modify) ---- */
bool   str_starts_with(const char *s, const char *prefix);
bool   str_ends_with(const char *s, const char *suffix);
size_t str_count(const char *s, const char *needle);         /* non-overlapping */
bool   str_equals_ignore_case(const char *a, const char *b);

/* ---- in-place changes (s must be writable) ---- */
char *str_trim(char *s);                    /* removes leading/trailing whitespace, returns s */
char *str_to_upper(char *s);
char *str_to_lower(char *s);
char *str_reverse(char *s);

/* ---- new strings: caller frees ---- */
char  *str_dup(const char *s);
char  *str_replace(const char *s, const char *from, const char *to);
char  *str_join(const char *const *parts, size_t count, const char *separator);
char **str_split(const char *s, char delimiter, size_t *count);   /* free with str_free_list */
void   str_free_list(char **list, size_t count);

/* ---- StrBuilder: growable text ---- */
typedef struct {
    char  *data;      /* always '\0'-terminated when not NULL */
    size_t length;
    size_t capacity;
} StrBuilder;

void  sb_init(StrBuilder *sb);
bool  sb_append(StrBuilder *sb, const char *text);
bool  sb_append_char(StrBuilder *sb, char c);
bool  sb_appendf(StrBuilder *sb, const char *format, ...);
char *sb_take(StrBuilder *sb);             /* hands ownership of the string to the caller */
void  sb_free(StrBuilder *sb);

#endif /* STRKIT_H */
