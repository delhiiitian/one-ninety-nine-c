CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

wcount: main.o counter.o
	$(CC) $(CFLAGS) main.o counter.o -o $@

test_counter: test_counter.o counter.o
	$(CC) $(CFLAGS) test_counter.o counter.o -o $@

%.o: %.c counter.h
	$(CC) $(CFLAGS) -c $< -o $@

test: wcount test_counter
	./test_counter
	./wcount sample.txt
	./wcount -lwL sample.txt main.c
	./wcount -t 5 sample.txt
	printf 'one two\nthree\n' | ./wcount -w
	./wcount missing.txt 2>/dev/null; test $$? -eq 1
	@if command -v wc >/dev/null; then \
	  a=$$(./wcount sample.txt | awk '{print $$1,$$2,$$3}'); \
	  b=$$(wc sample.txt | awk '{print $$1,$$2,$$3}'); \
	  test "$$a" = "$$b" && echo "matches system wc: $$a"; fi

clean:
	rm -f *.o wcount test_counter

.PHONY: test clean
