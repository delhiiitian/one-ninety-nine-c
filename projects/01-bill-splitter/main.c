/*
 * Project 1 - Bill Splitter
 * Asks for the bill, tip and tax, then prints a receipt and what each person owes.
 * Run: make && ./bill_splitter
 */
#include <stdio.h>
#include "bill.h"

/* Ask until the user types a number in [min, max]. */
static double ask_number(const char *question, double min, double max)
{
    double value;
    for (;;) {
        printf("%s ", question);
        if (scanf("%lf", &value) == 1 && value >= min && value <= max)
            return value;
        if (feof(stdin)) {               /* input ended: use the minimum */
            printf("\n");
            return min;
        }
        printf("  Please enter a number from %.0f to %.0f.\n", min, max);
        int c;
        while ((c = getchar()) != '\n' && c != EOF)   /* throw away the bad line */
            ;
    }
}

static void print_money(const char *label, long cents)
{
    printf("  %-22s $%7ld.%02ld\n", label, cents / 100, cents % 100);
}

int main(void)
{
    printf("=== Bill Splitter ===\n");
    Bill bill;
    bill.subtotal_cents = dollars_to_cents(ask_number("Bill amount before tax ($):", 0, 1000000));
    bill.tax_percent    = (int)ask_number("Tax percent (e.g. 8):", 0, 100);
    bill.tip_percent    = (int)ask_number("Tip percent (e.g. 15):", 0, 100);
    bill.people         = (int)ask_number("Number of people:", 1, 1000);

    Split s = split_bill(bill);

    printf("\n--- Receipt ---\n");
    print_money("Subtotal", bill.subtotal_cents);
    print_money("Tax", s.tax_cents);
    print_money("Tip", s.tip_cents);
    print_money("Total", s.total_cents);
    printf("\n--- Each person pays ---\n");
    for (int i = 1; i <= bill.people; i++) {
        long owed = s.share_cents + (i <= s.remainder_cents ? 1 : 0);
        char label[32];
        snprintf(label, sizeof label, "Person %d", i);
        print_money(label, owed);
        if (i == 10 && bill.people > 12) {
            printf("  ... and %d more people pay about the same\n", bill.people - 10);
            break;
        }
    }
    return 0;
}
