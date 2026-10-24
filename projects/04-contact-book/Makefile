CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

contacts: main.o contacts.o
	$(CC) $(CFLAGS) main.o contacts.o -o $@

test_contacts: test_contacts.o contacts.o
	$(CC) $(CFLAGS) test_contacts.o contacts.o -o $@

%.o: %.c contacts.h
	$(CC) $(CFLAGS) -c $< -o $@

test: contacts test_contacts
	./test_contacts
	printf 'list\nadd\nMei Lin\n+44 20 7946 0000\nmei@example.org\nfamily\nsearch\nexample\ndelete\nDev Patel\nlist\nquit\n' | ./contacts

clean:
	rm -f *.o contacts test_contacts

.PHONY: test clean
