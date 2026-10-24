CFLAGS = -std=c11 -Wall -Wextra -Werror
fields: main.o geometry.o stats.o
	gcc main.o geometry.o stats.o -lm -o fields
main.o: main.c geometry.h stats.h
	gcc $(CFLAGS) -c main.c
geometry.o: geometry.c geometry.h
	gcc $(CFLAGS) -c geometry.c
stats.o: stats.c stats.h
	gcc $(CFLAGS) -c stats.c
test: fields
	./fields
clean:
	rm -f *.o fields
.PHONY: test clean
