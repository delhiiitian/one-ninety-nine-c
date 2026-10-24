CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g

ledger: main.o ledger.o
	$(CC) $(CFLAGS) main.o ledger.o -o $@

test_ledger: test_ledger.o ledger.o
	$(CC) $(CFLAGS) test_ledger.o ledger.o -o $@

%.o: %.c ledger.h
	$(CC) $(CFLAGS) -c $< -o $@

# the sample has 6 deliberate errors, so the ledger must exit with 1
test: ledger test_ledger
	./test_ledger
	./ledger sample.txt; test $$? -eq 1
	./ledger missing.txt 2>/dev/null; test $$? -eq 2

# rebuild everything with AddressSanitizer and UndefinedBehaviorSanitizer
asan: clean
	$(MAKE) CFLAGS="$(CFLAGS) -fsanitize=address,undefined" test

clean:
	rm -f *.o ledger test_ledger

.PHONY: test asan clean
