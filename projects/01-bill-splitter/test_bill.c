/* test_bill.c - checks the maths with known answers. Run: make test */
#include <assert.h>
#include <stdio.h>
#include "bill.h"

int main(void)
{
    assert(dollars_to_cents(19.99) == 1999);
    assert(dollars_to_cents(0.1) == 10);

    Bill b = { 10000, 15, 8, 4 };         /* $100, 15% tip, 8% tax, 4 people */
    Split s = split_bill(b);
    assert(s.tax_cents == 800);
    assert(s.tip_cents == 1500);
    assert(s.total_cents == 12300);
    assert(s.share_cents == 3075 && s.remainder_cents == 0);

    Bill odd = { 1000, 0, 0, 3 };         /* $10 between 3: 3.34 + 3.33 + 3.33 */
    Split t = split_bill(odd);
    assert(t.share_cents == 333 && t.remainder_cents == 1);

    printf("All bill tests passed.\n");
    return 0;
}
