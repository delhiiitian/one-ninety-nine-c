temps: main.c temperature.c temperature.h
	gcc -std=c11 -Wall -Wextra -Werror main.c temperature.c -o temps
test: temps
	./temps
clean:
	rm -f temps
.PHONY: test clean
