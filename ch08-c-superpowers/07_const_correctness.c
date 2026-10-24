/*
 * Chapter 8.7 - const correctness: promises the compiler enforces
 */
#include <stdio.h>
#include <string.h>

typedef struct { char name[32]; int stock; double price; } Product;

/* const Product *: "I will only look" - callers can pass anything safely */
double stock_value(const Product *p)
{
    /* p->stock = 0;   <- compile error: p points to const data */
    return p->stock * p->price;
}

/* no const: "I will change it" - the signature tells the reader */
void restock(Product *p, int amount)
{
    p->stock += amount;
}

/* const on arrays of input, plain pointer for output */
void copy_upper(char *dest, const char *src, size_t size)
{
    size_t i = 0;
    for (; src[i] && i + 1 < size; i++)
        dest[i] = (src[i] >= 'a' && src[i] <= 'z') ? (char)(src[i] - 32) : src[i];
    dest[i] = '\0';
}

int main(void)
{
    Product p = { "Desk lamp", 12, 24.99 };
    printf("value of %s stock: $%.2f\n", p.name, stock_value(&p));
    restock(&p, 8);
    printf("after restock: %d in stock\n", p.stock);

    const int max_retries = 3;          /* a const variable */
    /* max_retries = 4;  <- error */

    const char *message = "read-only literal";   /* pointer to const char: text cannot change */
    message = "but the pointer can move";         /* fine */

    char buffer[32] = "editable";
    char *const fixed = buffer;         /* const pointer: cannot move, text CAN change */
    fixed[0] = 'E';
    /* fixed = NULL;  <- error */

    const char *const locked = "neither can change";   /* both */

    char shout[32];
    copy_upper(shout, p.name, sizeof shout);
    printf("%d retries | %s | %s | %s | %s\n", max_retries, message, fixed, locked, shout);
    return 0;
}
