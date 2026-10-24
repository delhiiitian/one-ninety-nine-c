/*
 * Chapter 4.2 - Two-dimensional arrays: tables and grids
 */
#include <stdio.h>

#define ROWS 3
#define COLS 3

int main(void)
{
    /* a tic-tac-toe board */
    char board[ROWS][COLS] = {
        { 'X', 'O', 'X' },
        { ' ', 'X', 'O' },
        { 'O', ' ', 'X' },
    };
    for (int r = 0; r < ROWS; r++) {
        printf(" %c | %c | %c\n", board[r][0], board[r][1], board[r][2]);
        if (r < ROWS - 1)
            printf("---+---+---\n");
    }

    /* the main diagonal: board[0][0], board[1][1], board[2][2] */
    int diagonal_win = board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2];
    printf("Diagonal win for %c? %s\n\n", board[0][0], diagonal_win ? "yes" : "no");

    /* sales per shop (rows) per quarter (columns) */
    int sales[3][4] = { { 120, 150, 170, 200 }, { 90, 95, 110, 130 }, { 300, 280, 310, 350 } };
    const char *shops[] = { "Delhi", "Pune", "Mumbai" };
    printf("%-8s %5s %5s %5s %5s %6s\n", "Shop", "Q1", "Q2", "Q3", "Q4", "Total");
    for (int s = 0; s < 3; s++) {
        int total = 0;
        printf("%-8s", shops[s]);
        for (int q = 0; q < 4; q++) {
            printf(" %5d", sales[s][q]);
            total += sales[s][q];
        }
        printf(" %6d\n", total);
    }
    return 0;
}
