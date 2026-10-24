/*
 * Chapter 1.1 - Hello, World
 * The smallest complete C program: include a library, define main, print, return.
 *
 * Build: gcc -Wall -Wextra -std=c11 01_hello_world.c -o hello
 * Run:   ./hello
 */
#include <stdio.h>   /* gives us printf() */

int main(void)       /* every C program starts running here */
{
    /* printf prints text; \n moves to a new line */
    printf("Hello, World!\n");
    printf("Welcome to C in 199 Pages.\n");

    return 0;        /* 0 tells the operating system "all good" */
}
