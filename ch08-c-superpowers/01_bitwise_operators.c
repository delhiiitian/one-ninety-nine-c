/*
 * Chapter 8.1 - Bitwise operators: working with individual bits
 * &  AND   |  OR   ^  XOR   ~  NOT   << shift left   >> shift right
 */
#include <stdio.h>
#include <stdint.h>

static void print_bits(const char *label, uint8_t v)
{
    printf("%-10s ", label);
    for (int bit = 7; bit >= 0; bit--)
        putchar((v >> bit) & 1 ? '1' : '0');
    printf("  (%3u)\n", v);
}

int main(void)
{
    uint8_t a = 0xCA;   /* 1100 1010 */
    uint8_t b = 0x0F;   /* 0000 1111 */

    print_bits("a", a);
    print_bits("b", b);
    print_bits("a & b", a & b);     /* keep only bits set in both: a "mask" */
    print_bits("a | b", a | b);     /* bits set in either */
    print_bits("a ^ b", a ^ b);     /* bits that differ */
    print_bits("~a", (uint8_t)~a);  /* flip every bit */
    print_bits("a << 1", (uint8_t)(a << 1));
    print_bits("a >> 4", a >> 4);

    /* practical tricks */
    unsigned n = 37;
    printf("\n%u is %s (n & 1)\n", n, (n & 1) ? "odd" : "even");
    printf("%u * 8 = %u (n << 3), %u / 4 = %u (n >> 2)\n", n, n << 3, n, n >> 2);

    unsigned x = 64;
    printf("%u is a power of two: %s (x & (x - 1)) == 0\n", x, (x & (x - 1)) == 0 ? "yes" : "no");

    /* pull the red, green and blue parts out of a colour like 0xFF8800 */
    uint32_t orange = 0xFF8800;
    printf("orange: red=%u green=%u blue=%u\n",
           (orange >> 16) & 0xFF, (orange >> 8) & 0xFF, orange & 0xFF);

    /* XOR swap and XOR "encryption" (toy only) */
    char secret[] = "MEET AT NOON";
    for (char *p = secret; *p; p++) *p ^= 0x2A;
    for (char *p = secret; *p; p++) *p ^= 0x2A;   /* XOR twice restores it */
    printf("after XOR twice: %s\n", secret);

    /* count set bits */
    unsigned v = 0xF0F0, count = 0;
    for (unsigned t = v; t; t >>= 1)
        count += t & 1;
    printf("0x%X has %u bits set\n", v, count);
    return 0;
}
