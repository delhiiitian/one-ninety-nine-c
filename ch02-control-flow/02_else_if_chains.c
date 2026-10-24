/*
 * Chapter 2.2 - else if chains: picking one of many paths
 * Converts an exam score into a grade. Try: echo 83 | ./else_if_chains
 */
#include <stdio.h>

int main(void)
{
    int score = 0;
    printf("Exam score (0-100): ");
    if (scanf("%d", &score) != 1)
        score = 83;
    printf("\n");

    char grade;
    if (score < 0 || score > 100) {
        printf("%d is not a valid score.\n", score);
        return 1;
    } else if (score >= 90) {
        grade = 'A';
    } else if (score >= 80) {          /* only checked when score < 90 */
        grade = 'B';
    } else if (score >= 70) {
        grade = 'C';
    } else if (score >= 60) {
        grade = 'D';
    } else {
        grade = 'F';
    }
    printf("Score %d -> grade %c\n", score, grade);
    return 0;
}
