/*
 * Chapter 3.3 - Function prototypes (declarations)
 * A prototype promises the compiler a function exists, so main can call it
 * before its full definition appears further down the file.
 */
#include <stdio.h>

/* prototypes: name, return type and parameter types, ending in ; */
int  add_vat(int price_cents, int vat_percent);
void print_price(const char *label, int cents);

int main(void)
{
    int coffee = 350;
    print_price("Coffee before VAT", coffee);
    print_price("Coffee with 20% VAT", add_vat(coffee, 20));
    return 0;
}

/* definitions can now come after main */
int add_vat(int price_cents, int vat_percent)
{
    return price_cents + (price_cents * vat_percent + 50) / 100;
}

void print_price(const char *label, int cents)
{
    printf("%-22s $%d.%02d\n", label, cents / 100, cents % 100);
}
