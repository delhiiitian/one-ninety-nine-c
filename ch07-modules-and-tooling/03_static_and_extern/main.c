/*
 * Chapter 7.3 - static and extern: controlling what other files can see
 */
#include <stdio.h>
#include "settings.h"

int main(void)
{
    verbose = 1;                       /* allowed: verbose is extern (shared) */
    settings_set_volume(7);
    settings_set_volume(42);           /* clamped to 10 by the module */
    /* volume = 99;  <- would not compile/link: volume is static inside settings.c */

    if (verbose)
        printf("volume is %d after %d changes\n", settings_volume(), settings_changes());
    return 0;
}
