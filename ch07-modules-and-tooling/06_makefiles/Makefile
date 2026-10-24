# Chapter 7.6 - an annotated Makefile
# A rule is:   target: prerequisites
#              <TAB>command
# make rebuilds a target only when a prerequisite is newer than it.

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -O2
LDFLAGS =
SRCS    = main.c greet.c
OBJS    = $(SRCS:.c=.o)          # main.o greet.o
TARGET  = greeter

# the first rule is the default: plain "make" builds it
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@        # $@ = the target name

# pattern rule: how to make any .o from its .c ($< = first prerequisite)
%.o: %.c greet.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) | grep -q "Good morning, Asha!"
	@echo "greeter test passed"

clean:
	rm -f $(OBJS) $(TARGET)

# these targets are not files
.PHONY: run test clean
