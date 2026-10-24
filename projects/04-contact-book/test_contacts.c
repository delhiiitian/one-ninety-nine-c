/* test_contacts.c - Run: make test */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "contacts.h"

int main(void)
{
    ContactBook b;
    book_init(&b);
    for (int i = 0; i < 10; i++) {                 /* forces several reallocs */
        char name[16];
        snprintf(name, sizeof name, "Person %d", i);
        assert(book_add(&b, name, "123", "p@x.com", FRIEND) == 0);
    }
    assert(b.count == 10 && b.capacity >= 10);
    assert(book_add(&b, "person 3", "", "", WORK) == -2);    /* duplicate, case-insensitive */
    assert(book_add(&b, "", "", "", WORK) == -3);

    assert(book_find(&b, "PERSON 7") != NULL);
    assert(book_remove(&b, "Person 7") == 0);
    assert(book_find(&b, "Person 7") == NULL && b.count == 9);
    assert(book_remove(&b, "Nobody") == -1);

    book_add(&b, "Aaron", "999", "aaron@mail.com", FAMILY);
    book_sort(&b);
    assert(strcmp(b.items[0].name, "Aaron") == 0);

    const Contact *r[5];
    assert(book_search(&b, "MAIL.COM", r, 5) == 1 && r[0]->group == FAMILY);
    assert(parse_group("Work") == WORK && parse_group("???") == OTHER);

    book_free(&b);
    assert(b.items == NULL && b.count == 0);
    printf("All contact tests passed.\n");
    return 0;
}
