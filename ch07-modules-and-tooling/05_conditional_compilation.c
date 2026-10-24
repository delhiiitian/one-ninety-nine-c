/*
 * Chapter 7.5 - Conditional compilation: one source, several builds
 *   gcc 05_conditional_compilation.c -o app               (release)
 *   gcc -DDEBUG 05_conditional_compilation.c -o app_dbg   (debug messages on)
 *   gcc -DMAX_ITEMS=3 05_conditional_compilation.c -o app (override a setting)
 */
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(...) fprintf(stderr, "debug: " __VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)       /* compiles to nothing in release builds */
#endif

#ifndef MAX_ITEMS
#define MAX_ITEMS 5                      /* default unless set with -D */
#endif

#if MAX_ITEMS > 100
#error "MAX_ITEMS is too large for this device"
#endif

static const char *platform(void)
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "an unknown system";
#endif
}

int main(void)
{
    printf("Built for %s, MAX_ITEMS = %d\n", platform(), MAX_ITEMS);
    for (int i = 0; i < MAX_ITEMS; i++) {
        DEBUG_PRINT("processing item %d\n", i);
        printf("item %d ", i);
    }
    printf("\n");
#ifdef DEBUG
    printf("This is a DEBUG build.\n");
#else
    printf("This is a release build (try -DDEBUG).\n");
#endif
    return 0;
}
