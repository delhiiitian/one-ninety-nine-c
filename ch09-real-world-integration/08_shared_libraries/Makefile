CFLAGS = -std=c11 -Wall -Wextra -Werror

# -fPIC: position-independent code, required for shared libraries
# -shared: produce a .so instead of a program
libtax.so: tax_india.c tax.h
	gcc $(CFLAGS) -fPIC -shared tax_india.c -o $@

libtax_uk.so: tax_uk.c tax.h
	gcc $(CFLAGS) -fPIC -shared tax_uk.c -o $@

app: main.c libtax.so
	gcc $(CFLAGS) main.c -L. -ltax -ldl -o $@

test: app libtax_uk.so
	LD_LIBRARY_PATH=. ./app
	LD_LIBRARY_PATH=. ./app ./libtax.so
	LD_LIBRARY_PATH=. ldd ./app | grep libtax

clean:
	rm -f *.so app
.PHONY: test clean
