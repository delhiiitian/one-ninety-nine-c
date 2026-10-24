CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

bill_splitter: main.o bill.o
	$(CC) $(CFLAGS) main.o bill.o -o $@

test_bill: test_bill.o bill.o
	$(CC) $(CFLAGS) test_bill.o bill.o -o $@

%.o: %.c bill.h
	$(CC) $(CFLAGS) -c $< -o $@

test: bill_splitter test_bill
	./test_bill
	printf '86.40\n8\n18\n3\n' | ./bill_splitter

clean:
	rm -f *.o bill_splitter test_bill

.PHONY: test clean
