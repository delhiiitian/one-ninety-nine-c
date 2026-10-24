/*
 * Chapter 8.3 - Memory layout: where things live, and struct padding
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int global_counter = 1;              /* data segment: lives for the whole program */
static const char banner[] = "hi";   /* read-only data */

struct Padded {      /* fields in a careless order */
    char   flag;     /* 1 byte, then 7 bytes of padding so 'value' is 8-aligned */
    double value;    /* 8 bytes */
    char   tag;      /* 1 byte, then 7 bytes of padding at the end */
};

struct Packed {      /* same fields, largest first */
    double value;
    char   flag;
    char   tag;      /* only 6 bytes of padding at the end */
};

int main(void)
{
    int local = 2;                         /* stack: gone when main returns */
    int *heap = malloc(sizeof *heap);      /* heap: lives until free() */
    if (heap == NULL) return 1;
    *heap = 3;

    printf("code   (main)          %p\n", (void *)(uintptr_t)main);
    printf("rodata (banner)        %p\n", (void *)banner);
    printf("data   (global)        %p\n", (void *)&global_counter);
    printf("heap   (malloc)        %p\n", (void *)heap);
    printf("stack  (local)         %p\n", (void *)&local);

    printf("\nstruct Padded: %zu bytes (flag@%zu value@%zu tag@%zu)\n", sizeof(struct Padded),
           offsetof(struct Padded, flag), offsetof(struct Padded, value), offsetof(struct Padded, tag));
    printf("struct Packed: %zu bytes (value@%zu flag@%zu tag@%zu)\n", sizeof(struct Packed),
           offsetof(struct Packed, value), offsetof(struct Packed, flag), offsetof(struct Packed, tag));
    printf("A million Padded structs waste %zu KB compared with Packed\n",
           (sizeof(struct Padded) - sizeof(struct Packed)) * 1000000 / 1024);

    /* byte order: how a 4-byte int is laid out in memory */
    uint32_t word = 0x11223344;
    const unsigned char *bytes = (const unsigned char *)&word;
    printf("\n0x11223344 in memory: %02X %02X %02X %02X -> %s-endian machine\n",
           bytes[0], bytes[1], bytes[2], bytes[3], bytes[0] == 0x44 ? "little" : "big");
    free(heap);
    return 0;
}
