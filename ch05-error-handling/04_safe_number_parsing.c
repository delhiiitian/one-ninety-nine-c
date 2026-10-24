/*
 * Chapter 5.4 - Safe number parsing with fgets + strtol
 * atoi("12abc") quietly gives 12 and atoi("abc") gives 0. strtol tells you what went wrong.
 */
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* returns 0 and stores the number on success, -1 on any problem */
int parse_int(const char *text, int min, int max, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);

    if (end == text)                return -1;   /* no digits at all */
    while (*end == ' ' || *end == '\n') end++;   /* allow trailing spaces */
    if (*end != '\0')               return -1;   /* junk after the number */
    if (errno == ERANGE)            return -1;   /* too big for a long */
    if (value < min || value > max) return -1;   /* outside what we accept */
    *out = (int)value;
    return 0;
}

int main(void)
{
    const char *tests[] = { "42", "  17\n", "12abc", "abc", "", "-5", "99999999999", "250" };
    for (int i = 0; i < 8; i++) {
        int age;
        if (parse_int(tests[i], 0, 130, &age) == 0)
            printf("%-14s -> age %d\n", tests[i][0] ? tests[i] : "(empty)", age);
        else
            printf("%-14s -> rejected (atoi would say %d)\n",
                   tests[i][0] ? tests[i] : "(empty)", atoi(tests[i]));
    }

    /* reading from the user safely: whole line first, then parse */
    char line[64];
    printf("\nHow many tickets (1-10)? ");
    int tickets;
    while (fgets(line, sizeof line, stdin) != NULL) {
        if (parse_int(line, 1, 10, &tickets) == 0) {
            printf("\nBooking %d ticket(s).\n", tickets);
            return 0;
        }
        printf("\n'%.*s' is not 1-10, try again: ", (int)strcspn(line, "\n"), line);
    }
    printf("\nNo valid answer given.\n");
    return 0;
}
