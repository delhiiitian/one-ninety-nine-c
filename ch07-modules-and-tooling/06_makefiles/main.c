/*
 * Chapter 7.6 - Makefiles
 * Run "make" to build, "make run", "make test", "make clean".
 * Change only greet.c and run make again: only greet.o is rebuilt.
 */
#include "greet.h"

int main(void)
{
    greet("Asha", 9);
    greet("Ravi", 15);
    greet("Mei", 21);
    return 0;
}
