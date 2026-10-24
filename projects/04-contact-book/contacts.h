/* contacts.h - an in-memory contact book backed by a growable array */
#ifndef CONTACTS_H
#define CONTACTS_H

#include <stddef.h>

#define NAME_LEN  48
#define PHONE_LEN 20
#define EMAIL_LEN 64

typedef enum { FRIEND, FAMILY, WORK, OTHER } Group;

typedef struct {
    char  name[NAME_LEN];
    char  phone[PHONE_LEN];
    char  email[EMAIL_LEN];
    Group group;
} Contact;

typedef struct {
    Contact *items;      /* malloc'd array */
    size_t   count;
    size_t   capacity;
} ContactBook;

void        book_init(ContactBook *book);
void        book_free(ContactBook *book);
int         book_add(ContactBook *book, const char *name, const char *phone,
                     const char *email, Group group);
int         book_remove(ContactBook *book, const char *name);
Contact    *book_find(ContactBook *book, const char *name);
size_t      book_search(const ContactBook *book, const char *text,
                        const Contact **results, size_t max_results);
void        book_sort(ContactBook *book);
const char *group_name(Group g);
Group       parse_group(const char *text);

#endif
