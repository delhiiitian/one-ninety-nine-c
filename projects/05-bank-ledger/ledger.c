/* ledger.c - every function returns a LedgerError; nothing here prints or exits */
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "ledger.h"

#define MAX_AMOUNT_CENTS 100000000000L   /* $1,000,000,000.00 per transaction */

const char *ledger_error_message(LedgerError e)
{
    static const char *messages[] = {
        [LEDGER_OK]              = "ok",
        [ERR_UNKNOWN_COMMAND]    = "unknown command",
        [ERR_BAD_ARGUMENTS]      = "wrong number of arguments",
        [ERR_BAD_AMOUNT]         = "amount must look like 12 or 12.50 and be positive",
        [ERR_NO_SUCH_ACCOUNT]    = "no such account",
        [ERR_ACCOUNT_EXISTS]     = "account already exists",
        [ERR_TOO_MANY_ACCOUNTS]  = "too many accounts",
        [ERR_INSUFFICIENT_FUNDS] = "insufficient funds",
        [ERR_SAME_ACCOUNT]       = "cannot transfer to the same account",
        [ERR_OVERFLOW]           = "balance would overflow",
    };
    if ((size_t)e < sizeof messages / sizeof messages[0] && messages[e] != NULL)
        return messages[e];
    return "unknown error";
}

/* strict parser: digits, optional '.' and 1-2 decimals. No signs, no junk, no floats. */
LedgerError parse_amount(const char *text, long *cents)
{
    long whole = 0;
    int digits = 0;
    const char *p = text;
    for (; isdigit((unsigned char)*p); p++, digits++) {
        if (whole > (MAX_AMOUNT_CENTS / 100))
            return ERR_BAD_AMOUNT;
        whole = whole * 10 + (*p - '0');
    }
    if (digits == 0)
        return ERR_BAD_AMOUNT;
    long fraction = 0;
    if (*p == '.') {
        p++;
        int decimals = 0;
        for (; isdigit((unsigned char)*p) && decimals < 2; p++, decimals++)
            fraction = fraction * 10 + (*p - '0');
        if (decimals == 0)
            return ERR_BAD_AMOUNT;
        if (decimals == 1)
            fraction *= 10;              /* "12.5" means 12.50 */
    }
    if (*p != '\0')
        return ERR_BAD_AMOUNT;           /* trailing junk or a third decimal */
    long total = whole * 100 + fraction;
    if (total <= 0 || total > MAX_AMOUNT_CENTS)
        return ERR_BAD_AMOUNT;
    *cents = total;
    return LEDGER_OK;
}

Account *ledger_find(Ledger *l, const char *name)
{
    for (size_t i = 0; i < l->count; i++)
        if (strcmp(l->accounts[i].name, name) == 0)
            return &l->accounts[i];
    return NULL;
}

LedgerError ledger_open(Ledger *l, const char *name, long initial_cents)
{
    if (strlen(name) >= ACCOUNT_NAME_LEN)
        return ERR_BAD_ARGUMENTS;
    if (ledger_find(l, name) != NULL)
        return ERR_ACCOUNT_EXISTS;
    if (l->count == MAX_ACCOUNTS)
        return ERR_TOO_MANY_ACCOUNTS;
    Account *a = &l->accounts[l->count++];
    snprintf(a->name, sizeof a->name, "%s", name);
    a->balance_cents = initial_cents;
    a->transactions = 0;
    return LEDGER_OK;
}

LedgerError ledger_deposit(Ledger *l, const char *name, long cents)
{
    Account *a = ledger_find(l, name);
    if (a == NULL)
        return ERR_NO_SUCH_ACCOUNT;
    if (a->balance_cents > LONG_MAX - cents)
        return ERR_OVERFLOW;
    a->balance_cents += cents;
    a->transactions++;
    return LEDGER_OK;
}

LedgerError ledger_withdraw(Ledger *l, const char *name, long cents)
{
    Account *a = ledger_find(l, name);
    if (a == NULL)
        return ERR_NO_SUCH_ACCOUNT;
    if (cents > a->balance_cents)
        return ERR_INSUFFICIENT_FUNDS;
    a->balance_cents -= cents;
    a->transactions++;
    return LEDGER_OK;
}

/* all checks happen BEFORE any change, so a failed transfer changes nothing */
LedgerError ledger_transfer(Ledger *l, const char *from, const char *to, long cents)
{
    Account *src = ledger_find(l, from);
    Account *dst = ledger_find(l, to);
    if (src == NULL || dst == NULL)
        return ERR_NO_SUCH_ACCOUNT;
    if (src == dst)
        return ERR_SAME_ACCOUNT;
    if (cents > src->balance_cents)
        return ERR_INSUFFICIENT_FUNDS;
    if (dst->balance_cents > LONG_MAX - cents)
        return ERR_OVERFLOW;
    src->balance_cents -= cents;
    dst->balance_cents += cents;
    src->transactions++;
    dst->transactions++;
    return LEDGER_OK;
}

/* parse one command line and run it; a human-readable result goes into reply */
LedgerError ledger_execute(Ledger *l, const char *line, char *reply, size_t reply_size)
{
    char cmd[16], a[ACCOUNT_NAME_LEN + 8], b[ACCOUNT_NAME_LEN + 8], c[32], extra[2];
    int n = sscanf(line, "%15s %31s %31s %31s %1s", cmd, a, b, c, extra);
    long cents = 0;
    LedgerError e;
    reply[0] = '\0';

    if (n <= 0)
        return LEDGER_OK;                               /* blank line */
    if (strcmp(cmd, "open") == 0) {
        if (n != 2 && n != 3) return ERR_BAD_ARGUMENTS;
        if (n == 3 && (e = parse_amount(b, &cents)) != LEDGER_OK) return e;
        if ((e = ledger_open(l, a, cents)) != LEDGER_OK) return e;
        snprintf(reply, reply_size, "opened %s with %ld.%02ld", a, cents / 100, cents % 100);
    } else if (strcmp(cmd, "deposit") == 0 || strcmp(cmd, "withdraw") == 0) {
        if (n != 3) return ERR_BAD_ARGUMENTS;
        if ((e = parse_amount(b, &cents)) != LEDGER_OK) return e;
        e = cmd[0] == 'd' ? ledger_deposit(l, a, cents) : ledger_withdraw(l, a, cents);
        if (e != LEDGER_OK) return e;
        Account *acc = ledger_find(l, a);
        snprintf(reply, reply_size, "%s balance %ld.%02ld", a,
                 acc->balance_cents / 100, acc->balance_cents % 100);
    } else if (strcmp(cmd, "transfer") == 0) {
        if (n != 4) return ERR_BAD_ARGUMENTS;
        if ((e = parse_amount(c, &cents)) != LEDGER_OK) return e;
        if ((e = ledger_transfer(l, a, b, cents)) != LEDGER_OK) return e;
        snprintf(reply, reply_size, "moved %ld.%02ld from %s to %s", cents / 100, cents % 100, a, b);
    } else if (strcmp(cmd, "balance") == 0) {
        if (n != 2) return ERR_BAD_ARGUMENTS;
        Account *acc = ledger_find(l, a);
        if (acc == NULL) return ERR_NO_SUCH_ACCOUNT;
        snprintf(reply, reply_size, "%s balance %ld.%02ld", a,
                 acc->balance_cents / 100, acc->balance_cents % 100);
    } else {
        return ERR_UNKNOWN_COMMAND;
    }
    return LEDGER_OK;
}
