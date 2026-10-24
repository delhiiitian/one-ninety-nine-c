/* query.c - functions that only read strings */
#include <ctype.h>
#include <string.h>
#include "strkit.h"

bool str_starts_with(const char *s, const char *prefix)
{
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

bool str_ends_with(const char *s, const char *suffix)
{
    size_t n = strlen(s), m = strlen(suffix);
    return m <= n && memcmp(s + n - m, suffix, m) == 0;
}

size_t str_count(const char *s, const char *needle)
{
    size_t m = strlen(needle), count = 0;
    if (m == 0)
        return 0;
    for (const char *p = strstr(s, needle); p != NULL; p = strstr(p + m, needle))
        count++;
    return count;
}

bool str_equals_ignore_case(const char *a, const char *b)
{
    for (; *a && *b; a++, b++)
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
    return *a == *b;
}
