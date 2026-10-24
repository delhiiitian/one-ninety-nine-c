CFLAGS = -std=c11 -Wall -Wextra -Werror
LIB_OBJS = number_theory.o primes.o

primes_demo: main.c libmathx.a mathx.h
	gcc $(CFLAGS) main.c -L. -lmathx -o $@

libmathx.a: $(LIB_OBJS)
	ar rcs $@ $(LIB_OBJS)

%.o: %.c mathx.h
	gcc $(CFLAGS) -c $< -o $@

test: primes_demo
	./primes_demo
	ar t libmathx.a

clean:
	rm -f *.o *.a primes_demo
.PHONY: test clean
