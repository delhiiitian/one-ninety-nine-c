CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -O2
LDFLAGS = -pthread

tinyweb: main.o server.o http.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_http: test_http.o http.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c http.h server.h
	$(CC) $(CFLAGS) -c $< -o $@

run: tinyweb
	./tinyweb public 8080

test: test_http tinyweb
	./test_http
	sh test_server.sh

clean:
	rm -f *.o tinyweb test_http server.log

.PHONY: run test clean
