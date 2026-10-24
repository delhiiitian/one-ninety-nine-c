/* builder.c - StrBuilder, a growable string */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strkit.h"

void sb_init(StrBuilder *sb)
{
    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;
}

/* make room for `extra` more characters plus the '\0' */
static bool reserve(StrBuilder *sb, size_t extra)
{
    size_t needed = sb->length + extra + 1;
    if (needed <= sb->capacity)
        return true;
    size_t cap = sb->capacity ? sb->capacity : 16;
    while (cap < needed)
        cap *= 2;
    char *bigger = realloc(sb->data, cap);
    if (bigger == NULL)
        return false;
    sb->data = bigger;
    sb->capacity = cap;
    return true;
}

bool sb_append(StrBuilder *sb, const char *text)
{
    size_t n = strlen(text);
    if (!reserve(sb, n))
        return false;
    memcpy(sb->data + sb->length, text, n + 1);
    sb->length += n;
    return true;
}

bool sb_append_char(StrBuilder *sb, char c)
{
    if (!reserve(sb, 1))
        return false;
    sb->data[sb->length++] = c;
    sb->data[sb->length] = '\0';
    return true;
}

bool sb_appendf(StrBuilder *sb, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int n = vsnprintf(NULL, 0, format, args);    /* first pass: measure */
    va_end(args);
    if (n < 0 || !reserve(sb, (size_t)n))
        return false;
    va_start(args, format);
    vsnprintf(sb->data + sb->length, (size_t)n + 1, format, args);   /* second pass: write */
    va_end(args);
    sb->length += (size_t)n;
    return true;
}

char *sb_take(StrBuilder *sb)
{
    char *s = sb->data;
    sb_init(sb);                 /* the builder no longer owns it */
    return s;
}

void sb_free(StrBuilder *sb)
{
    free(sb->data);
    sb_init(sb);
}
