CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

convert: main.o units.o
	$(CC) $(CFLAGS) main.o units.o -o $@

test_units: test_units.o units.o
	$(CC) $(CFLAGS) test_units.o units.o -o $@ -lm

%.o: %.c units.h
	$(CC) $(CFLAGS) -c $< -o $@

test: convert test_units
	./test_units
	./convert 100 F C
	printf 'list\n5 km mi\n70 kg lb\n3 kg m\n1 parsec m\nquit\n' | ./convert

clean:
	rm -f *.o convert test_units

.PHONY: test clean
