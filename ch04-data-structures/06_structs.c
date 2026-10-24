/*
 * Chapter 4.6 - Structs: grouping related values into one type
 */
#include <stdio.h>
#include <string.h>

typedef struct {
    char   title[64];
    char   author[48];
    int    year;
    double price;
    int    in_stock;
} Book;

void print_book(const Book *b)            /* pass a pointer: no copy of 128+ bytes */
{
    printf("%-30s %-20s %4d  $%6.2f  %s\n", b->title, b->author, b->year, b->price,
           b->in_stock ? "in stock" : "sold out");
}

void apply_sale(Book *b, int percent)
{
    b->price = b->price * (100 - percent) / 100.0;   /* -> reaches a field through a pointer */
}

int main(void)
{
    Book shelf[] = {
        { "The C Programming Language", "Kernighan & Ritchie", 1978, 59.99, 1 },
        { "Clean Code", "Robert Martin", 2008, 39.50, 0 },
        { "Code", "Charles Petzold", 1999, 24.95, 1 },
    };
    int count = sizeof shelf / sizeof shelf[0];

    Book extra;                                       /* fill one field by field */
    strcpy(extra.title, "Hacker's Delight");
    strcpy(extra.author, "Henry Warren");
    extra.year = 2002;
    extra.price = 49.0;
    extra.in_stock = 1;

    apply_sale(&shelf[0], 20);
    for (int i = 0; i < count; i++)
        print_book(&shelf[i]);
    print_book(&extra);

    Book copy = extra;                                /* assigning copies every field */
    copy.price = 1.0;
    printf("Copy costs %.2f, original still %.2f\n", copy.price, extra.price);
    printf("One Book uses %zu bytes\n", sizeof(Book));
    return 0;
}
