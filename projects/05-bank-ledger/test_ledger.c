/* test_ledger.c - Run: make test */
#include <assert.h>
#include <stdio.h>
#include "ledger.h"

int main(void)
{
    long c;
    assert(parse_amount("12", &c) == LEDGER_OK && c == 1200);
    assert(parse_amount("12.5", &c) == LEDGER_OK && c == 1250);
    assert(parse_amount("0.07", &c) == LEDGER_OK && c == 7);
    assert(parse_amount("12.345", &c) == ERR_BAD_AMOUNT);
    assert(parse_amount("-3", &c) == ERR_BAD_AMOUNT);
    assert(parse_amount("abc", &c) == ERR_BAD_AMOUNT);
    assert(parse_amount("0", &c) == ERR_BAD_AMOUNT);
    assert(parse_amount("12.", &c) == ERR_BAD_AMOUNT);
    assert(parse_amount("99999999999999999999", &c) == ERR_BAD_AMOUNT);

    static Ledger l;
    assert(ledger_open(&l, "a", 1000) == LEDGER_OK);
    assert(ledger_open(&l, "b", 0) == LEDGER_OK);
    assert(ledger_open(&l, "a", 0) == ERR_ACCOUNT_EXISTS);
    assert(ledger_withdraw(&l, "a", 1001) == ERR_INSUFFICIENT_FUNDS);
    assert(ledger_find(&l, "a")->balance_cents == 1000);      /* failure changed nothing */
    assert(ledger_transfer(&l, "a", "b", 400) == LEDGER_OK);
    assert(ledger_find(&l, "a")->balance_cents == 600 && ledger_find(&l, "b")->balance_cents == 400);
    assert(ledger_transfer(&l, "a", "zzz", 1) == ERR_NO_SUCH_ACCOUNT);
    assert(ledger_transfer(&l, "b", "b", 1) == ERR_SAME_ACCOUNT);

    char reply[128];
    assert(ledger_execute(&l, "deposit b 1.00\n", reply, sizeof reply) == LEDGER_OK);
    assert(ledger_execute(&l, "deposit b\n", reply, sizeof reply) == ERR_BAD_ARGUMENTS);
    assert(ledger_execute(&l, "fly b 1\n", reply, sizeof reply) == ERR_UNKNOWN_COMMAND);
    assert(ledger_execute(&l, "\n", reply, sizeof reply) == LEDGER_OK);

    for (int i = 2; i < MAX_ACCOUNTS; i++) {
        char name[8];
        snprintf(name, sizeof name, "x%d", i);
        assert(ledger_open(&l, name, 0) == LEDGER_OK);
    }
    assert(ledger_open(&l, "onemore", 0) == ERR_TOO_MANY_ACCOUNTS);
    printf("All ledger tests passed.\n");
    return 0;
}
