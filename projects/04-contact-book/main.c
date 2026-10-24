/*
 * Project 4 - Contact Book
 * Commands: add, list, find, search, delete, help, quit
 */
#include <stdio.h>
#include <string.h>
#include "contacts.h"

/* read one line, strip the newline; returns 0 at end of input */
static int read_line(const char *prompt, char *buf, size_t size)
{
    printf("%s", prompt);
    if (fgets(buf, (int)size, stdin) == NULL)
        return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

static void print_contact(const Contact *c)
{
    printf("  %-20s %-15s %-28s %s\n", c->name, c->phone, c->email, group_name(c->group));
}

static void cmd_add(ContactBook *book)
{
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN], group[16];
    if (!read_line("  Name:  ", name, sizeof name) || !read_line("  Phone: ", phone, sizeof phone) ||
        !read_line("  Email: ", email, sizeof email) ||
        !read_line("  Group (friend/family/work/other): ", group, sizeof group))
        return;
    switch (book_add(book, name, phone, email, parse_group(group))) {
    case 0:  printf("  Added %s.\n", name); break;
    case -2: printf("  %s is already in the book.\n", name); break;
    case -3: printf("  A contact needs a name.\n"); break;
    default: printf("  Out of memory.\n"); break;
    }
}

static void cmd_list(ContactBook *book)
{
    book_sort(book);
    printf("  %zu contact(s)\n", book->count);
    for (size_t i = 0; i < book->count; i++)
        print_contact(&book->items[i]);
}

static void cmd_search(const ContactBook *book)
{
    char text[64];
    if (!read_line("  Search for: ", text, sizeof text))
        return;
    const Contact *results[20];
    size_t n = book_search(book, text, results, 20);
    printf("  %zu match(es)\n", n);
    for (size_t i = 0; i < n; i++)
        print_contact(results[i]);
}

static void cmd_delete(ContactBook *book)
{
    char name[NAME_LEN];
    if (!read_line("  Name to delete: ", name, sizeof name))
        return;
    if (book_remove(book, name) == 0)
        printf("  Deleted %s.\n", name);
    else
        printf("  No contact called %s.\n", name);
}

int main(void)
{
    ContactBook book;
    book_init(&book);
    book_add(&book, "Asha Rao", "+91 98100 12345", "asha@example.com", FRIEND);
    book_add(&book, "Dev Patel", "+1 415 555 0100", "dev@work.example", WORK);

    printf("=== Contact Book === (type 'help')\n");
    char cmd[32];
    while (read_line("> ", cmd, sizeof cmd)) {
        if (strcmp(cmd, "add") == 0)            cmd_add(&book);
        else if (strcmp(cmd, "list") == 0)      cmd_list(&book);
        else if (strcmp(cmd, "search") == 0 || strcmp(cmd, "find") == 0) cmd_search(&book);
        else if (strcmp(cmd, "delete") == 0)    cmd_delete(&book);
        else if (strcmp(cmd, "quit") == 0)      break;
        else if (strcmp(cmd, "help") == 0 || cmd[0] == '\0')
            printf("  Commands: add, list, search, delete, quit\n");
        else
            printf("  Unknown command '%s'. Type 'help'.\n", cmd);
    }
    book_free(&book);
    printf("Goodbye.\n");
    return 0;
}
