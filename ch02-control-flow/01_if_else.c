/*
 * Chapter 2.1 - if / else: choosing between two paths
 * Try: echo 72 | ./if_else
 */
#include <stdio.h>

int main(void)
{
    int battery = 0;
    printf("Phone battery level (0-100): ");
    if (scanf("%d", &battery) != 1)
        battery = 15;                     /* no input: pretend 15% */
    printf("\n");

    if (battery < 20) {
        printf("Battery low (%d%%). Turning on power saver.\n", battery);
    } else {
        printf("Battery fine (%d%%). Full performance.\n", battery);
    }

    /* a single statement does not need braces, but braces avoid bugs later */
    if (battery == 100)
        printf("Fully charged: you can unplug.\n");

    return 0;
}
