# Project 2: Number Guessing Game

The computer picks a secret number and you guess it with "Higher!" and "Lower!" hints. Three levels, a limited number of tries, a narrowing hint range, and a score that rewards fewer tries.

## Files

| File | Job |
| --- | --- |
| `game.h` | `Game`, `Level` and `Verdict` types and the rule functions |
| `game.c` | The rules: new game, check a guess, score |
| `main.c` | The menu, the rounds and all printing |
| `test_game.c` | Tests the rules with a known secret |
| `Makefile` | `make` builds `./guess`, `make test` runs the tests |

## Build and run

```sh
make
./guess                 # random secret
./guess --secret 42     # fixed secret, handy for testing
make test
```

## Ideas to extend it

- Save the best score to a file (Chapter 6).
- Let two players take turns and compare scores.
