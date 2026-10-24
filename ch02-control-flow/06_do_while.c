/*
 * Chapter 2.6 - do-while: run first, check after
 * Perfect for menus and "ask until valid" input.
 * Try: printf '7\n3\n' | ./do_while
 */
#include <stdio.h>

int main(void)
{
    int choice;
    int ok;
    do {
        printf("Pick a size (1 small, 2 medium, 3 large): ");
        ok = scanf("%d", &choice);
        if (ok != 1) {               /* no number: stop instead of looping forever */
            choice = 2;
            break;
        }
        printf("\n");
        if (choice < 1 || choice > 3)
            printf("%d is not on the menu, try again.\n", choice);
    } while (choice < 1 || choice > 3);   /* note the semicolon */

    const char *names[] = { "", "small", "medium", "large" };
    printf("You picked a %s coffee.\n", names[choice]);
    return 0;
}
