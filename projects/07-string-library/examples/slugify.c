/*
 * slugify - turns titles into URL "slugs" using strkit
 *   ./slugify "  Hello, World: C in 199 Pages!  "   ->  hello-world-c-in-199-pages
 * With no arguments it converts a few sample titles.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "strkit.h"

static char *slugify(const char *title)
{
    char *copy = str_dup(title);
    if (copy == NULL)
        return NULL;
    str_to_lower(str_trim(copy));

    StrBuilder sb;
    sb_init(&sb);
    int pending_dash = 0;
    for (const char *p = copy; *p; p++) {
        if (isalnum((unsigned char)*p)) {
            if (pending_dash && sb.length > 0)
                sb_append_char(&sb, '-');
            sb_append_char(&sb, *p);
            pending_dash = 0;
        } else {
            pending_dash = 1;          /* any run of other characters becomes one dash */
        }
    }
    free(copy);
    return sb.data ? sb_take(&sb) : str_dup("");
}

int main(int argc, char *argv[])
{
    const char *samples[] = { "  Hello, World: C in 199 Pages!  ", "Top 10 Tips -- for C++ & C#",
                              "Already-a-slug" };
    int n = argc > 1 ? argc - 1 : 3;
    for (int i = 0; i < n; i++) {
        const char *title = argc > 1 ? argv[i + 1] : samples[i];
        char *slug = slugify(title);
        printf("%-36s -> %s\n", title, slug ? slug : "(out of memory)");
        free(slug);
    }
    printf("(strkit %s)\n", STRKIT_VERSION);
    return 0;
}
