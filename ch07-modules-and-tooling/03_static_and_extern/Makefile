volume: main.c settings.c settings.h
	gcc -std=c11 -Wall -Wextra -Werror main.c settings.c -o volume
test: volume
	./volume
clean:
	rm -f volume
.PHONY: test clean
