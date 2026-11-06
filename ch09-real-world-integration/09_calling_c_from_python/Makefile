libfastmath.so: fastmath.c
	gcc -std=c11 -Wall -Wextra -Werror -O2 -fPIC -shared fastmath.c -o $@
test: libfastmath.so
	python3 use_fastmath.py
clean:
	rm -f libfastmath.so
.PHONY: test clean
