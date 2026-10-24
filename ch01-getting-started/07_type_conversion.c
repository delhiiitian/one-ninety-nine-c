/*
 * Chapter 1.7 - Type conversion and casting
 * C converts between types automatically in some places; a cast does it on purpose.
 */
#include <stdio.h>

int main(void)
{
    int    total_marks = 437, subjects = 5;

    double wrong = total_marks / subjects;           /* int/int happens first: 87 */
    double right = (double)total_marks / subjects;   /* cast first: 87.4 */
    printf("Average without cast: %.2f\n", wrong);
    printf("Average with cast:    %.2f\n", right);

    double price = 19.99;
    int    whole_dollars = (int)price;               /* cast cuts off, never rounds */
    int    rounded = (int)(price + 0.5);             /* simple rounding for positives */
    printf("(int)19.99 = %d, rounded = %d\n", whole_dollars, rounded);

    char letter = 'A';
    int  code = letter;                              /* char is a small integer */
    printf("'%c' has code %d; next letter is '%c'\n", letter, code, letter + 1);

    unsigned char small = (unsigned char)300;        /* too big: wraps around */
    printf("300 stored in an unsigned char becomes %d\n", small);
    return 0;
}
