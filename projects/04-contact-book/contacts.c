/* contacts.c - storage and search; no user interface here */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contacts.h"

void book_init(ContactBook *book)
{
    book->items = NULL;
    book->count = 0;
    book->capacity = 0;
}

void book_free(ContactBook *book)
{
    free(book->items);
    book_init(book);
}

static int grow(ContactBook *book)
{
    size_t new_cap = book->capacity == 0 ? 4 : book->capacity * 2;
    Contact *bigger = realloc(book->items, new_cap * sizeof *bigger);
    if (bigger == NULL)
        return -1;
    book->items = bigger;
    book->capacity = new_cap;
    return 0;
}

/* copies text into a fixed-size field, always '\0'-terminated */
static void copy_field(char *dest, size_t size, const char *src)
{
    snprintf(dest, size, "%s", src);
}

/* returns 0 on success, -1 out of memory, -2 duplicate name, -3 empty name */
int book_add(ContactBook *book, const char *name, const char *phone,
             const char *email, Group group)
{
    if (name[0] == '\0')
        return -3;
    if (book_find(book, name) != NULL)
        return -2;
    if (book->count == book->capacity && grow(book) != 0)
        return -1;
    Contact *c = &book->items[book->count++];
    copy_field(c->name, sizeof c->name, name);
    copy_field(c->phone, sizeof c->phone, phone);
    copy_field(c->email, sizeof c->email, email);
    c->group = group;
    return 0;
}

static int same_text_ignoring_case(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

Contact *book_find(ContactBook *book, const char *name)
{
    for (size_t i = 0; i < book->count; i++)
        if (same_text_ignoring_case(book->items[i].name, name))
            return &book->items[i];
    return NULL;
}

int book_remove(ContactBook *book, const char *name)
{
    Contact *c = book_find(book, name);
    if (c == NULL)
        return -1;
    size_t index = (size_t)(c - book->items);          /* pointer difference = index */
    memmove(&book->items[index], &book->items[index + 1],
            (book->count - index - 1) * sizeof *book->items);   /* close the gap */
    book->count--;
    return 0;
}

static int contains_ignoring_case(const char *haystack, const char *needle)
{
    size_t n = strlen(needle);
    for (; *haystack; haystack++) {
        size_t i = 0;
        while (i < n && haystack[i] &&
               tolower((unsigned char)haystack[i]) == tolower((unsigned char)needle[i]))
            i++;
        if (i == n)
            return 1;
    }
    return n == 0;
}

size_t book_search(const ContactBook *book, const char *text,
                   const Contact **results, size_t max_results)
{
    size_t found = 0;
    for (size_t i = 0; i < book->count && found < max_results; i++) {
        const Contact *c = &book->items[i];
        if (contains_ignoring_case(c->name, text) || contains_ignoring_case(c->phone, text) ||
            contains_ignoring_case(c->email, text))
            results[found++] = c;
    }
    return found;
}

static int compare_by_name(const void *a, const void *b)
{
    const Contact *x = a, *y = b;
    for (const char *p = x->name, *q = y->name;; p++, q++) {
        int d = tolower((unsigned char)*p) - tolower((unsigned char)*q);
        if (d != 0 || *p == '\0')
            return d;
    }
}

void book_sort(ContactBook *book)
{
    if (book->count > 1)
        qsort(book->items, book->count, sizeof *book->items, compare_by_name);
}

const char *group_name(Group g)
{
    static const char *names[] = { "friend", "family", "work", "other" };
    return g <= OTHER ? names[g] : "other";
}

Group parse_group(const char *text)
{
    for (int g = FRIEND; g <= OTHER; g++)
        if (same_text_ignoring_case(text, group_name((Group)g)))
            return (Group)g;
    return OTHER;
}
