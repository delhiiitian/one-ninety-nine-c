/*
 * Chapter 1.2 - From source code to a running program
 * C is compiled: the compiler turns this text file into a machine-code program.
 * This program prints facts the compiler baked in at build time.
 *
 * See every stage yourself:
 *   gcc -E 02_compile_and_run.c -o stage1.i   (preprocess: #include pasted in)
 *   gcc -S 02_compile_and_run.c -o stage2.s   (compile: assembly language)
 *   gcc -c 02_compile_and_run.c -o stage3.o   (assemble: machine code object)
 *   gcc stage3.o -o build_info                (link: a runnable program)
 */
#include <stdio.h>

int main(void)
{
    printf("This file:        %s\n", __FILE__);
    printf("Compiled on:      %s at %s\n", __DATE__, __TIME__);
    printf("C standard level: %ld\n", (long)__STDC_VERSION__);
#ifdef __GNUC__
    printf("Compiler:         GCC-compatible %d.%d\n", __GNUC__, __GNUC_MINOR__);
#endif
    printf("The program ran, so compile + link worked.\n");
    return 0;
}
