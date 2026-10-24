/* test_game.c - checks the rules. Run: make test */
#include <assert.h>
#include <stdio.h>
#include "game.h"

int main(void)
{
    Game g = new_game(MEDIUM, 42);
    assert(g.max_number == 100 && g.max_tries == 8);

    assert(check_guess(&g, 50) == TOO_HIGH);
    assert(g.high == 49);
    assert(check_guess(&g, 25) == TOO_LOW);
    assert(g.low == 26);
    assert(check_guess(&g, 42) == CORRECT);
    assert(g.tries_used == 3);
    assert(score_for(&g) == (8 - 3 + 1) * 10 * 2);

    Game h = new_game(HARD, 7);
    assert(h.max_number == 1000);
    check_guess(&h, 7);
    assert(score_for(&h) == 10 * 10 * 3);

    printf("All game tests passed.\n");
    return 0;
}
