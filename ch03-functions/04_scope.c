/*
 * Chapter 3.4 - Scope: where a name is visible
 * Local variables live inside their braces; global variables are visible everywhere in the file.
 */
#include <stdio.h>

int visitors_today = 0;           /* global: any function in this file can use it */

void welcome(const char *name)
{
    int greeting_number = ++visitors_today;   /* local: exists only during this call */
    printf("Welcome %s, you are visitor #%d\n", name, greeting_number);
}

int main(void)
{
    welcome("Ravi");
    welcome("Mei");

    int total = 10;               /* local to main */
    {
        int total = 99;           /* a NEW variable that hides the outer one (shadowing) */
        printf("Inner block sees total = %d\n", total);
    }
    printf("Outer block sees total = %d\n", total);

    for (int i = 0; i < 2; i++) {
        int squared = i * i;      /* i and squared vanish after the loop */
        printf("i=%d squared=%d\n", i, squared);
    }
    /* printf("%d", i);  <- error: i is out of scope here */

    printf("Visitors counted by the global: %d\n", visitors_today);
    return 0;
}
