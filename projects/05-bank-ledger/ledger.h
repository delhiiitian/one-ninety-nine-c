/* ledger.h - accounts and transactions with explicit error codes */
#ifndef LEDGER_H
#define LEDGER_H

#include <stddef.h>

#define MAX_ACCOUNTS 64
#define ACCOUNT_NAME_LEN 24

typedef enum {
    LEDGER_OK = 0,
    ERR_UNKNOWN_COMMAND,
    ERR_BAD_ARGUMENTS,
    ERR_BAD_AMOUNT,
    ERR_NO_SUCH_ACCOUNT,
    ERR_ACCOUNT_EXISTS,
    ERR_TOO_MANY_ACCOUNTS,
    ERR_INSUFFICIENT_FUNDS,
    ERR_SAME_ACCOUNT,
    ERR_OVERFLOW
} LedgerError;

typedef struct {
    char name[ACCOUNT_NAME_LEN];
    long balance_cents;
    int  transactions;
} Account;

typedef struct {
    Account accounts[MAX_ACCOUNTS];
    size_t  count;
} Ledger;

const char *ledger_error_message(LedgerError e);
LedgerError parse_amount(const char *text, long *cents);
LedgerError ledger_open(Ledger *l, const char *name, long initial_cents);
LedgerError ledger_deposit(Ledger *l, const char *name, long cents);
LedgerError ledger_withdraw(Ledger *l, const char *name, long cents);
LedgerError ledger_transfer(Ledger *l, const char *from, const char *to, long cents);
Account    *ledger_find(Ledger *l, const char *name);
LedgerError ledger_execute(Ledger *l, const char *line, char *reply, size_t reply_size);

#endif
