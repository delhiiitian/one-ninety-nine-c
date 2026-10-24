/*
 * Chapter 4.9 - enums and unions
 * enum: a named set of choices. union: one box that holds ONE of several types at a time.
 */
#include <stdio.h>

typedef enum { PAY_CASH, PAY_CARD, PAY_UPI } PaymentKind;

/* a "tagged union": the enum says which member of the union is valid */
typedef struct {
    PaymentKind kind;
    double      amount;
    union {
        struct { double tendered; }            cash;
        struct { char last4[5]; int expiry; }   card;
        struct { char handle[32]; }            upi;
    } details;
} Payment;

void describe(const Payment *p)
{
    printf("$%.2f ", p->amount);
    switch (p->kind) {
    case PAY_CASH:
        printf("cash, change $%.2f\n", p->details.cash.tendered - p->amount);
        break;
    case PAY_CARD:
        printf("card ending %s, expires %d\n", p->details.card.last4, p->details.card.expiry);
        break;
    case PAY_UPI:
        printf("UPI from %s\n", p->details.upi.handle);
        break;
    }
}

int main(void)
{
    Payment payments[] = {
        { PAY_CASH, 18.50, .details.cash = { 20.00 } },
        { PAY_CARD, 42.00, .details.card = { "4242", 1228 } },
        { PAY_UPI,  7.25,  .details.upi  = { "asha@okbank" } },
    };
    for (int i = 0; i < 3; i++)
        describe(&payments[i]);

    /* a union is as big as its largest member, because members share memory */
    printf("sizeof details union = %zu bytes (largest member: upi handle)\n",
           sizeof payments[0].details);
    return 0;
}
