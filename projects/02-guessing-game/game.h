/* game.h - rules of the number guessing game, separate from input and output */
#ifndef GAME_H
#define GAME_H

typedef enum { TOO_LOW, TOO_HIGH, CORRECT } Verdict;

typedef enum { EASY, MEDIUM, HARD } Level;

typedef struct {
    int secret;
    int max_number;     /* secret is in 1..max_number */
    int max_tries;
    int tries_used;
    int low, high;      /* the range still possible, shown as a hint */
} Game;

Game    new_game(Level level, int secret);
Verdict check_guess(Game *game, int guess);
int     score_for(const Game *game);

#endif
