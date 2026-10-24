/*
 * Chapter 4.3 - Strings: arrays of characters ending in '\0'
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    char city[] = "Delhi";            /* 6 bytes: D e l h i \0 */
    printf("'%s' has length %zu but uses %zu bytes\n", city, strlen(city), sizeof city);

    char greeting[32];
    /* snprintf builds a string safely: it never writes past sizeof greeting */
    snprintf(greeting, sizeof greeting, "Hello from %s!", city);
    printf("%s\n", greeting);

    /* comparing: use strcmp, never == */
    const char *password = "secret123";
    char attempt[] = "secret123";
    printf("Passwords match: %s\n", strcmp(password, attempt) == 0 ? "yes" : "no");

    /* walking a string character by character until '\0' */
    char shout[32];
    size_t i;
    for (i = 0; greeting[i] != '\0' && i < sizeof shout - 1; i++)
        shout[i] = (char)toupper((unsigned char)greeting[i]);
    shout[i] = '\0';                  /* always end with the terminator */
    printf("%s\n", shout);

    /* searching */
    const char *email = "asha@example.com";
    const char *at = strchr(email, '@');
    if (at != NULL)
        printf("Username is %.*s, domain is %s\n", (int)(at - email), email, at + 1);

    /* counting vowels */
    int vowels = 0;
    for (const char *p = "programming in c"; *p; p++)
        if (strchr("aeiou", *p))
            vowels++;
    printf("Vowels in 'programming in c': %d\n", vowels);
    return 0;
}
