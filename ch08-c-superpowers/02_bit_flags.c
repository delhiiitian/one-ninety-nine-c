/*
 * Chapter 8.2 - Bit flags: many yes/no settings in one integer
 */
#include <stdio.h>
#include <stdint.h>

/* each permission is one bit, like Unix file permissions */
enum {
    PERM_READ    = 1u << 0,   /* 0001 */
    PERM_WRITE   = 1u << 1,   /* 0010 */
    PERM_EXECUTE = 1u << 2,   /* 0100 */
    PERM_SHARE   = 1u << 3,   /* 1000 */
};

static void show(const char *who, uint8_t perms)
{
    printf("%-7s %c%c%c%c\n", who,
           perms & PERM_READ    ? 'r' : '-',
           perms & PERM_WRITE   ? 'w' : '-',
           perms & PERM_EXECUTE ? 'x' : '-',
           perms & PERM_SHARE   ? 's' : '-');
}

/* bit-fields: let the compiler pack small fields into bits */
typedef struct {
    unsigned int is_on      : 1;
    unsigned int brightness : 7;   /* 0..127 */
    unsigned int colour     : 3;   /* 0..7 */
} LampState;

int main(void)
{
    uint8_t owner = PERM_READ | PERM_WRITE;    /* set two flags with OR */
    uint8_t guest = PERM_READ;

    owner |= PERM_EXECUTE;                     /* turn a flag ON */
    show("owner", owner);
    show("guest", guest);

    owner &= (uint8_t)~PERM_WRITE;             /* turn a flag OFF */
    show("owner", owner);

    guest ^= PERM_SHARE;                       /* TOGGLE a flag */
    show("guest", guest);

    if ((owner & (PERM_READ | PERM_EXECUTE)) == (PERM_READ | PERM_EXECUTE))
        printf("owner can read AND execute\n");

    /* Unix-style: 0754 = rwx for owner, r-x group, r-- others */
    unsigned mode = 0754;
    printf("mode %o: owner bits %o, group bits %o, other bits %o\n",
           mode, (mode >> 6) & 7, (mode >> 3) & 7, mode & 7);

    LampState lamp = { 1, 100, 5 };
    printf("lamp: on=%u brightness=%u colour=%u, stored in %zu bytes\n",
           lamp.is_on, lamp.brightness, lamp.colour, sizeof lamp);
    return 0;
}
