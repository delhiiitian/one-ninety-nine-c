/*
 * Chapter 9.3 - Environment variables: configuration from outside the program
 * Try: APP_PORT=9000 APP_DEBUG=1 ./environment_variables
 */
#define _POSIX_C_SOURCE 200809L   /* for setenv */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;                         /* every variable, as "NAME=value" strings */

/* read a number from the environment, with a default */
static long env_long(const char *name, long fallback)
{
    const char *text = getenv(name);           /* NULL if not set */
    if (text == NULL || *text == '\0')
        return fallback;
    char *end;
    long v = strtol(text, &end, 10);
    return *end == '\0' ? v : fallback;
}

int main(void)
{
    const char *home = getenv("HOME");
    const char *user = getenv("USER");
    printf("HOME = %s\n", home ? home : "(not set)");
    printf("USER = %s\n", user ? user : "(not set)");

    long port = env_long("APP_PORT", 8080);
    int debug = env_long("APP_DEBUG", 0) != 0;
    printf("Server would listen on port %ld, debug %s\n", port, debug ? "on" : "off");

    setenv("APP_MODE", "testing", 1);          /* visible to this program and its children */
    printf("APP_MODE = %s\n", getenv("APP_MODE"));

    int count = 0, path_like = 0;
    for (char **e = environ; *e != NULL; e++) {
        count++;
        if (strncmp(*e, "PATH=", 5) == 0)
            path_like = 1;
    }
    printf("%d variables in the environment, PATH %s\n", count, path_like ? "present" : "missing");
    return 0;
}
