/*
 * Chapter 5.1 - Return codes: how C functions report failure
 * C has no exceptions. A function returns a status, and the caller checks it.
 */
#include <stdio.h>

typedef enum {
    WITHDRAW_OK = 0,
    WITHDRAW_INVALID_AMOUNT,
    WITHDRAW_INSUFFICIENT_FUNDS,
    WITHDRAW_DAILY_LIMIT
} WithdrawResult;

const char *withdraw_message(WithdrawResult r)
{
    switch (r) {
    case WITHDRAW_OK:                 return "ok";
    case WITHDRAW_INVALID_AMOUNT:     return "amount must be positive";
    case WITHDRAW_INSUFFICIENT_FUNDS: return "insufficient funds";
    case WITHDRAW_DAILY_LIMIT:        return "daily limit reached";
    }
    return "unknown error";
}

/* the result goes out through a pointer; the return value is the status */
WithdrawResult withdraw(long *balance, long amount, long *withdrawn_today)
{
    const long DAILY_LIMIT = 50000;          /* $500.00 in cents */
    if (amount <= 0)
        return WITHDRAW_INVALID_AMOUNT;
    if (amount > *balance)
        return WITHDRAW_INSUFFICIENT_FUNDS;
    if (*withdrawn_today + amount > DAILY_LIMIT)
        return WITHDRAW_DAILY_LIMIT;
    *balance -= amount;
    *withdrawn_today += amount;
    return WITHDRAW_OK;
}

int main(void)
{
    long balance = 80000, today = 0;          /* $800.00 */
    long requests[] = { 20000, -500, 25000, 10000, 90000 };

    for (int i = 0; i < 5; i++) {
        WithdrawResult r = withdraw(&balance, requests[i], &today);
        if (r != WITHDRAW_OK) {               /* ALWAYS check */
            printf("Withdraw %6.2f failed: %s\n", requests[i] / 100.0, withdraw_message(r));
            continue;
        }
        printf("Withdraw %6.2f ok, balance %.2f\n", requests[i] / 100.0, balance / 100.0);
    }
    return 0;
}
