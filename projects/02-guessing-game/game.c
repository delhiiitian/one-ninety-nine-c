/* game.c - pure game logic */
#include "game.h"

Game new_game(Level level, int secret)
{
    Game g;
    switch (level) {
    case EASY:   g.max_number = 50;   g.max_tries = 10; break;
    case MEDIUM: g.max_number = 100;  g.max_tries = 8;  break;
    case HARD:   g.max_number = 1000; g.max_tries = 10; break;
    default:     g.max_number = 100;  g.max_tries = 8;  break;
    }
    g.secret = secret;
    g.tries_used = 0;
    g.low = 1;
    g.high = g.max_number;
    return g;
}

Verdict check_guess(Game *game, int guess)
{
    game->tries_used++;
    if (guess < game->secret) {
        if (guess >= game->low)
            game->low = guess + 1;      /* narrow the hint range */
        return TOO_LOW;
    }
    if (guess > game->secret) {
        if (guess <= game->high)
            game->high = guess - 1;
        return TOO_HIGH;
    }
    return CORRECT;
}

int score_for(const Game *game)
{
    /* fewer tries = more points; harder levels multiply the score */
    int left = game->max_tries - game->tries_used + 1;
    int multiplier = game->max_number >= 1000 ? 3 : game->max_number >= 100 ? 2 : 1;
    return left * 10 * multiplier;
}
