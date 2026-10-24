CFLAGS = -std=c11 -Wall -Wextra -Werror
test_password: test_password.c password.c password.h mini_test.h
	gcc $(CFLAGS) test_password.c password.c -o $@
test: test_password
	./test_password
clean:
	rm -f test_password
.PHONY: test clean
