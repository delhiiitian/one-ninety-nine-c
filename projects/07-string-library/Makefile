# Project 7 - strkit, a tested string library
CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -Iinclude
SRCS    = src/query.c src/transform.c src/builder.c
OBJS    = $(SRCS:src/%.c=build/%.o)
LIB     = build/libstrkit.a

all: $(LIB) build/slugify

# 1. compile each library file into build/
build/%.o: src/%.c include/strkit.h | build
	$(CC) $(CFLAGS) -c $< -o $@

# 2. bundle the objects into a static library
$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

# 3. programs link against the library
build/test_strkit: tests/test_strkit.c tests/mini_test.h $(LIB)
	$(CC) $(CFLAGS) $< -Lbuild -lstrkit -o $@

build/slugify: examples/slugify.c $(LIB)
	$(CC) $(CFLAGS) $< -Lbuild -lstrkit -o $@

build:
	mkdir -p build

test: build/test_strkit build/slugify
	./build/test_strkit
	./build/slugify

# run the tests under AddressSanitizer to prove there are no leaks or overflows
asan:
	rm -rf build
	$(MAKE) CFLAGS="$(CFLAGS) -g -fsanitize=address,undefined" test

clean:
	rm -rf build

.PHONY: all test asan clean
