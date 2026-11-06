/*
 * Chapter 9.2 - Random numbers: rand() for games, the OS for secrets
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* a fair number in [0, n): reject the uneven top part of rand()'s range */
int random_below(int n)
{
    int limit = RAND_MAX - (RAND_MAX % n);
    int r;
    do {
        r = rand();
    } while (r >= limit);
    return r % n;
}

/* Fisher-Yates shuffle: every order equally likely */
void shuffle(int *a, int n)
{
    for (int i = n - 1; i > 0; i--) {
        int j = random_below(i + 1);
        int t = a[i]; a[i] = a[j]; a[j] = t;
    }
}

/* for passwords, tokens and keys: ask the operating system */
int secure_bytes(unsigned char *buf, size_t n)
{
    FILE *f = fopen("/dev/urandom", "rb");
    if (f == NULL)
        return -1;
    size_t got = fread(buf, 1, n, f);
    fclose(f);
    return got == n ? 0 : -1;
}

int main(void)
{
    srand(42);                                 /* fixed seed: the same "random" numbers every run */
    printf("seed 42:  %d %d %d\n", rand() % 100, rand() % 100, rand() % 100);
    srand(42);
    printf("seed 42:  %d %d %d  (repeatable, useful for tests)\n", rand() % 100, rand() % 100, rand() % 100);

    srand((unsigned)time(NULL));               /* different each run */
    int counts[6] = { 0 };
    for (int i = 0; i < 60000; i++)
        counts[random_below(6)]++;
    printf("60000 dice rolls:");
    for (int f = 0; f < 6; f++)
        printf(" %d:%d", f + 1, counts[f]);
    printf("\n");

    int deck[10];
    for (int i = 0; i < 10; i++) deck[i] = i + 1;
    shuffle(deck, 10);
    printf("shuffled 1-10:");
    for (int i = 0; i < 10; i++) printf(" %d", deck[i]);
    printf("\n");

    unsigned char token[16];
    if (secure_bytes(token, sizeof token) == 0) {
        printf("secure token: ");
        for (int i = 0; i < 16; i++) printf("%02x", token[i]);
        printf("\n");
    } else {
        printf("no /dev/urandom on this system\n");
    }
    return 0;
}
