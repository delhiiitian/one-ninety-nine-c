/*
 * Chapter 5.6 - Exit codes, exit() and atexit()
 * The number main returns (or exit() receives) tells the shell whether you succeeded.
 *   ./exit_codes          -> exits 0
 *   ./exit_codes fail     -> exits 1   (check with: echo $?)
 *   ./exit_codes usage    -> exits 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { EXIT_USAGE = 2 };        /* many tools use 2 for "you called me wrongly" */

static void say_goodbye(void)
{
    printf("[atexit] cleanup ran before the program ended\n");
}

static void fatal(const char *message)
{
    fprintf(stderr, "error: %s\n", message);
    exit(EXIT_FAILURE);         /* ends the program from anywhere; atexit handlers still run */
}

int main(int argc, char *argv[])
{
    atexit(say_goodbye);         /* register a function to run at exit */

    if (argc > 2) {
        fprintf(stderr, "usage: %s [fail|usage]\n", argv[0]);
        return EXIT_USAGE;
    }
    if (argc == 2 && strcmp(argv[1], "fail") == 0)
        fatal("you asked me to fail");
    if (argc == 2 && strcmp(argv[1], "usage") == 0) {
        fprintf(stderr, "usage: %s [fail|usage]\n", argv[0]);
        return EXIT_USAGE;
    }

    printf("Everything worked, exiting with %d\n", EXIT_SUCCESS);
    return EXIT_SUCCESS;        /* same as exit(EXIT_SUCCESS) */
}
