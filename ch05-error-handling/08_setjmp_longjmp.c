/*
 * Chapter 5.8 - setjmp and longjmp: jumping out of deep calls
 * setjmp marks a spot; longjmp jumps back to it from any depth.
 * Like an emergency exit: use it rarely, and never skip cleanup you need.
 */
#include <setjmp.h>
#include <stdio.h>

static jmp_buf on_error;

/* a tiny calculator for expressions like "8 / 2 / 0" */
static double divide(double a, double b)
{
    if (b == 0)
        longjmp(on_error, 1);     /* abandon the whole calculation */
    return a / b;
}

static double evaluate(const double *numbers, int count)
{
    double result = numbers[0];
    for (int i = 1; i < count; i++)
        result = divide(result, numbers[i]);
    return result;
}

int main(void)
{
    double good[] = { 100, 5, 2 };
    double bad[]  = { 8, 2, 0, 4 };
    const double *jobs[] = { good, bad };
    int sizes[] = { 3, 4 };

    for (int j = 0; j < 2; j++) {
        if (setjmp(on_error) == 0) {          /* returns 0 the first time */
            printf("Job %d result: %g\n", j + 1, evaluate(jobs[j], sizes[j]));
        } else {                              /* returns 1 when longjmp lands here */
            printf("Job %d failed: division by zero\n", j + 1);
        }
    }
    return 0;
}
