/* bill.h - the money maths for the bill splitter, kept apart from the user interface */
#ifndef BILL_H
#define BILL_H

typedef struct {
    long subtotal_cents;   /* what the menu prices add up to */
    int  tip_percent;      /* e.g. 15 */
    int  tax_percent;      /* e.g. 8 */
    int  people;           /* how many ways to split */
} Bill;

typedef struct {
    long tax_cents;
    long tip_cents;
    long total_cents;
    long share_cents;      /* what most people pay */
    long remainder_cents;  /* leftover cents: the first few people pay 1 cent more */
} Split;

long  dollars_to_cents(double dollars);
Split split_bill(Bill bill);

#endif
