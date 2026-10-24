/* transform.c - in-place changes and functions that return new strings */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "strkit.h"

char *str_trim(char *s)
{
    char *start = s;
    while (isspace((unsigned char)*start))
        start++;
    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1]))
        end--;
    size_t len = (size_t)(end - start);
    memmove(s, start, len);           /* regions may overlap: memmove, not memcpy */
    s[len] = '\0';
    return s;
}

char *str_to_upper(char *s)
{
    for (char *p = s; *p; p++)
        *p = (char)toupper((unsigned char)*p);
    return s;
}

char *str_to_lower(char *s)
{
    for (char *p = s; *p; p++)
        *p = (char)tolower((unsigned char)*p);
    return s;
}

char *str_reverse(char *s)
{
    size_t n = strlen(s);
    for (size_t i = 0; i < n / 2; i++) {
        char t = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = t;
    }
    return s;
}

char *str_dup(const char *s)
{
    size_t n = strlen(s) + 1;
    char *copy = malloc(n);
    if (copy != NULL)
        memcpy(copy, s, n);
    return copy;
}

char *str_replace(const char *s, const char *from, const char *to)
{
    size_t from_len = strlen(from), to_len = strlen(to);
    if (from_len == 0)
        return str_dup(s);
    size_t hits = str_count(s, from);
    size_t out_len = strlen(s) - hits * from_len + hits * to_len;   /* exact size, measured first */
    char *out = malloc(out_len + 1);
    if (out == NULL)
        return NULL;
    char *w = out;
    const char *r = s, *hit;
    while ((hit = strstr(r, from)) != NULL) {
        memcpy(w, r, (size_t)(hit - r));          /* text before the match */
        w += hit - r;
        memcpy(w, to, to_len);                    /* the replacement */
        w += to_len;
        r = hit + from_len;
    }
    strcpy(w, r);                                 /* the rest, including '\0' */
    return out;
}

char *str_join(const char *const *parts, size_t count, const char *separator)
{
    StrBuilder sb;
    sb_init(&sb);
    for (size_t i = 0; i < count; i++) {
        if ((i > 0 && !sb_append(&sb, separator)) || !sb_append(&sb, parts[i])) {
            sb_free(&sb);
            return NULL;
        }
    }
    if (sb.data == NULL)
        return str_dup("");
    return sb_take(&sb);
}

char **str_split(const char *s, char delimiter, size_t *count)
{
    size_t n = 1;
    for (const char *p = s; *p; p++)
        if (*p == delimiter)
            n++;
    char **list = malloc(n * sizeof *list);
    if (list == NULL)
        return NULL;
    size_t i = 0;
    const char *start = s;
    for (;;) {
        const char *end = strchr(start, delimiter);
        size_t len = end ? (size_t)(end - start) : strlen(start);
        list[i] = malloc(len + 1);
        if (list[i] == NULL) {
            str_free_list(list, i);
            return NULL;
        }
        memcpy(list[i], start, len);
        list[i][len] = '\0';
        i++;
        if (end == NULL)
            break;
        start = end + 1;
    }
    *count = n;
    return list;
}

void str_free_list(char **list, size_t count)
{
    if (list == NULL)
        return;
    for (size_t i = 0; i < count; i++)
        free(list[i]);
    free(list);
}
