/*
 * Project 2 - Number Guessing Game
 * The computer picks a number; you guess with "higher/lower" hints.
 * Run: ./guess            (random number)
 *      ./guess --secret 42  (fixed number, for testing)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

static int read_int(int *out)
{
    while (scanf("%d", out) != 1) {
        if (feof(stdin))
            return 0;                   /* no more input */
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;                           /* discard the bad line */
        printf("Please type a whole number: ");
    }
    return 1;
}

static int play_round(int fixed_secret)
{
    printf("\nChoose a level: 1 easy (1-50), 2 medium (1-100), 3 hard (1-1000): ");
    int choice;
    if (!read_int(&choice))
        return -1;
    Level level = choice == 1 ? EASY : choice == 3 ? HARD : MEDIUM;

    Game g = new_game(level, 0);
    g.secret = fixed_secret > 0 ? fixed_secret : rand() % g.max_number + 1;
    printf("I picked a number from 1 to %d. You have %d tries.\n", g.max_number, g.max_tries);

    while (g.tries_used < g.max_tries) {
        printf("Try %d of %d (hint: %d-%d): ", g.tries_used + 1, g.max_tries, g.low, g.high);
        int guess;
        if (!read_int(&guess))
            return -1;
        if (guess < 1 || guess > g.max_number) {
            printf("  Out of range, that try does not count.\n");
            continue;
        }
        switch (check_guess(&g, guess)) {
        case TOO_LOW:  printf("  Higher!\n"); break;
        case TOO_HIGH: printf("  Lower!\n");  break;
        case CORRECT: {
            int points = score_for(&g);
            printf("  Correct! %d was my number. Tries: %d. Score: %d\n",
                   g.secret, g.tries_used, points);
            return points;
        }
        }
    }
    printf("  Out of tries. My number was %d.\n", g.secret);
    return 0;
}

int main(int argc, char *argv[])
{
    int fixed_secret = 0;
    if (argc == 3 && strcmp(argv[1], "--secret") == 0)
        fixed_secret = atoi(argv[2]);
    srand((unsigned)time(NULL));        /* different numbers each run */

    printf("=== Number Guessing Game ===\n");
    int total = 0, rounds = 0;
    char again = 'y';
    do {
        int points = play_round(fixed_secret);
        if (points < 0)
            break;
        total += points;
        rounds++;
        printf("Total score after %d round(s): %d\nPlay again? (y/n): ", rounds, total);
        if (scanf(" %c", &again) != 1)
            break;
    } while (again == 'y' || again == 'Y');

    printf("\nThanks for playing! Final score: %d\n", total);
    return 0;
}
