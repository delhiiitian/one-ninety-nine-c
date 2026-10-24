/*
 * Chapter 8.9 - inline functions and measuring performance
 * Build twice and compare:
 *   gcc -O0 09_inline_and_performance.c -o slow
 *   gcc -O2 09_inline_and_performance.c -o fast
 */
#define _POSIX_C_SOURCE 200809L   /* for clock_gettime with -std=c11 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2000

static inline uint8_t clamp_u8(int v)       /* tiny function: inline removes the call cost */
{
    return (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v);
}

static double now_seconds(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

static long sum_rows_first(int (*m)[N])      /* walks memory in order: cache friendly */
{
    long s = 0;
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            s += m[r][c];
    return s;
}

static long sum_columns_first(int (*m)[N])   /* jumps N*4 bytes each step: cache hostile */
{
    long s = 0;
    for (int c = 0; c < N; c++)
        for (int r = 0; r < N; r++)
            s += m[r][c];
    return s;
}

int main(void)
{
    int (*m)[N] = malloc(sizeof(int[N][N]));  /* 16 MB, too big for the stack */
    if (m == NULL) return 1;
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
            m[r][c] = (r + c) & 7;

    double t0 = now_seconds();
    long a = sum_rows_first(m);
    double t1 = now_seconds();
    long b = sum_columns_first(m);
    double t2 = now_seconds();
    printf("row-by-row    sum=%ld in %.3f s\n", a, t1 - t0);
    printf("column-first  sum=%ld in %.3f s  (same work, worse memory order)\n", b, t2 - t1);

    /* brightening an "image" with an inline clamp */
    static uint8_t pixels[1 << 20];
    for (size_t i = 0; i < sizeof pixels; i++) pixels[i] = (uint8_t)(i * 7);
    double t3 = now_seconds();
    for (int pass = 0; pass < 20; pass++)
        for (size_t i = 0; i < sizeof pixels; i++)
            pixels[i] = clamp_u8(pixels[i] + 10);
    double t4 = now_seconds();
    printf("brightened 20 x 1M pixels in %.3f s (pixel[0]=%u)\n", t4 - t3, pixels[0]);
    free(m);
    return 0;
}
