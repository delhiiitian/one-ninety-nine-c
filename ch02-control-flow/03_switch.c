/*
 * Chapter 2.3 - switch: a menu of exact choices
 * A vending machine that maps a button number to a drink.
 * Try: echo 2 | ./switch
 */
#include <stdio.h>

int main(void)
{
    int button = 0;
    printf("Vending machine: 1 Water  2 Cola  3 Juice  4 Coffee  5 Tea\n");
    printf("Press a button: ");
    if (scanf("%d", &button) != 1)
        button = 2;
    printf("\n");

    switch (button) {
    case 1:
        printf("Here is your water. $1.00\n");
        break;                        /* leave the switch */
    case 2:
        printf("Here is your cola. $1.50\n");
        break;
    case 3:
        printf("Here is your juice. $2.00\n");
        break;
    case 4:                           /* 4 and 5 share the same code (fall-through) */
    case 5:
        printf("Hot drinks take 30 seconds... done. $2.50\n");
        break;
    default:                          /* anything else */
        printf("Button %d does nothing. Coins returned.\n", button);
        break;
    }
    return 0;
}
