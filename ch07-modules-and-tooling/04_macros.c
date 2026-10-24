/*
 * Chapter 7.4 - Preprocessor macros: text substitution before compiling
 * See the result with: gcc -E 04_macros.c | tail -40
 */
#include <stdio.h>

#define MAX_USERS 100                          /* object-like macro: a named constant */
#define SQUARE_BAD(x) x * x                    /* function-like macro, WITHOUT brackets */
#define SQUARE(x) ((x) * (x))                  /* correct: bracket every use and the whole */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ARRAY_LEN(arr) (sizeof (arr) / sizeof (arr)[0])
#define STRINGIFY(x) #x                        /* # turns an argument into a string */
#define PRINT_INT(var) printf(#var " = %d\n", var)
#define MAKE_GETTER(field) int get_##field(void) { return field; }   /* ## glues tokens */

/* multi-line macro: do { ... } while (0) makes it behave like one statement */
#define LOG(level, msg) do { \
        printf("[%s] %s:%d %s\n", level, __FILE__, __LINE__, msg); \
    } while (0)

static int score = 42;
MAKE_GETTER(score)          /* expands to: int get_score(void) { return score; } */

int main(void)
{
    printf("MAX_USERS = %d\n", MAX_USERS);
    printf("SQUARE_BAD(2 + 3) = %d  (expands to 2 + 3 * 2 + 3)\n", SQUARE_BAD(2 + 3));
    printf("SQUARE(2 + 3)     = %d\n", SQUARE(2 + 3));
    printf("MAX(7, 12)        = %d\n", MAX(7, 12));

    int primes[] = { 2, 3, 5, 7, 11, 13 };
    printf("ARRAY_LEN(primes) = %zu\n", ARRAY_LEN(primes));

    printf("STRINGIFY(hello world) = \"%s\"\n", STRINGIFY(hello world));
    int lives = 3;
    PRINT_INT(lives);
    printf("get_score() = %d\n", get_score());

    if (lives > 0)
        LOG("INFO", "player still alive");
    else
        LOG("WARN", "game over");

    /* danger: arguments with side effects may be evaluated twice */
    int i = 5;
    int m = MAX(i++, 3);     /* i++ runs twice! */
    printf("MAX(i++, 3) gave %d and left i = %d (not 6)\n", m, i);
    return 0;
}
