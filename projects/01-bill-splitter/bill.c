/* bill.c - pure calculation, no printing, so it is easy to test */
#include "bill.h"

long dollars_to_cents(double dollars)
{
    /* add half a cent before truncating so 19.99 does not become 1998 */
    return (long)(dollars * 100.0 + 0.5);
}

Split split_bill(Bill bill)
{
    Split s;
    /* percentages of cents, rounded to the nearest cent */
    s.tax_cents = (bill.subtotal_cents * bill.tax_percent + 50) / 100;
    s.tip_cents = (bill.subtotal_cents * bill.tip_percent + 50) / 100;
    s.total_cents = bill.subtotal_cents + s.tax_cents + s.tip_cents;

    s.share_cents = s.total_cents / bill.people;
    s.remainder_cents = s.total_cents % bill.people;
    return s;
}
