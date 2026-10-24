/*
 * Chapter 2.4 - while: repeat while a condition is true
 * Savings: how many months until a goal is reached?
 */
#include <stdio.h>

int main(void)
{
    double balance = 0.0;
    const double monthly_deposit = 250.0;
    const double goal = 2000.0;
    const double monthly_interest = 0.004;   /* 0.4% a month */
    int months = 0;

    while (balance < goal) {                 /* checked BEFORE every round */
        balance += monthly_deposit;
        balance += balance * monthly_interest;
        months++;
        printf("Month %2d: $%8.2f\n", months, balance);
    }
    printf("Goal of $%.0f reached after %d months.\n", goal, months);

    /* a while loop may run zero times */
    int tickets_left = 0;
    while (tickets_left > 0) {
        printf("This never prints.\n");
    }
    printf("No tickets left, loop body skipped.\n");
    return 0;
}
