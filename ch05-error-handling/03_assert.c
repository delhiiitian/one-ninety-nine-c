/*
 * Chapter 5.3 - assert: catching programmer mistakes early
 * assert(condition) stops the program with file and line if the condition is false.
 * It checks for BUGS, not for bad user input. Compile with -DNDEBUG to remove all asserts.
 * Run "./assert --break" to see one fail.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* percent must be 0..100: anything else is a bug in the CALLER */
int apply_percent(int value, int percent)
{
    assert(percent >= 0 && percent <= 100);
    return value * percent / 100;
}

double average(const int *values, int count)
{
    assert(values != NULL);
    assert(count > 0);
    long sum = 0;
    for (int i = 0; i < count; i++)
        sum += values[i];
    return (double)sum / count;
}

int main(int argc, char *argv[])
{
    int marks[] = { 70, 85, 90 };
    printf("20%% of 450 = %d\n", apply_percent(450, 20));
    printf("average = %.2f\n", average(marks, 3));

    if (argc > 1 && strcmp(argv[1], "--break") == 0) {
        printf("Calling apply_percent with 150 ...\n");
        apply_percent(450, 150);              /* assertion fails here */
    }
    printf("All assertions held.\n");
    return 0;
}
