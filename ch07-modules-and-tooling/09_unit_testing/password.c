#include <ctype.h>
#include <string.h>
#include "password.h"

/* one point each for: length >= 8, length >= 12, lower, upper, digit, symbol */
Strength password_strength(const char *pw)
{
    size_t len = strlen(pw);
    int lower = 0, upper = 0, digit = 0, symbol = 0;
    for (const char *p = pw; *p; p++) {
        unsigned char c = (unsigned char)*p;
        if (islower(c)) lower = 1;
        else if (isupper(c)) upper = 1;
        else if (isdigit(c)) digit = 1;
        else symbol = 1;
    }
    int points = (len >= 8) + (len >= 12) + lower + upper + digit + symbol;
    if (len < 8 || points <= 3) return WEAK;
    if (points <= 4) return MEDIUM;
    return STRONG;
}

const char *strength_name(Strength s)
{
    return s == STRONG ? "strong" : s == MEDIUM ? "medium" : "weak";
}
