CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

guess: main.o game.o
	$(CC) $(CFLAGS) main.o game.o -o $@

test_game: test_game.o game.o
	$(CC) $(CFLAGS) test_game.o game.o -o $@

%.o: %.c game.h
	$(CC) $(CFLAGS) -c $< -o $@

test: guess test_game
	./test_game
	printf '2\n50\n25\n42\ny\n1\n10\n42\nn\n' | ./guess --secret 42

clean:
	rm -f *.o guess test_game

.PHONY: test clean
