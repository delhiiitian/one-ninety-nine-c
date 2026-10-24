/*
 * Chapter 1.6 - Operators: doing maths and comparisons
 */
#include <stdio.h>

int main(void)
{
    int a = 17, b = 5;

    printf("a + b  = %d\n", a + b);
    printf("a - b  = %d\n", a - b);
    printf("a * b  = %d\n", a * b);
    printf("a / b  = %d   (integer division drops the remainder)\n", a / b);
    printf("a %% b  = %d   (remainder)\n", a % b);
    printf("a / 5.0 = %.1f (one double makes it real division)\n", a / 5.0);

    int counter = 10;
    counter += 5;     /* same as counter = counter + 5 */
    counter *= 2;     /* 30 */
    counter++;        /* 31 */
    printf("counter after += 5, *= 2, ++ : %d\n", counter);

    /* comparisons produce 1 (true) or 0 (false) */
    printf("a > b  -> %d\n", a > b);
    printf("a == b -> %d\n", a == b);

    /* logical operators combine conditions */
    int age = 20, has_ticket = 1;
    printf("can enter (age >= 18 && has_ticket): %d\n", age >= 18 && has_ticket);

    /* precedence: * before +, use brackets to be clear */
    printf("2 + 3 * 4   = %d\n", 2 + 3 * 4);
    printf("(2 + 3) * 4 = %d\n", (2 + 3) * 4);
    return 0;
}
