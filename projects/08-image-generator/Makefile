CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -O2
OBJS   = image.o fractal.o bmp.o

imagegen: main.o $(OBJS)
	$(CC) $(CFLAGS) main.o $(OBJS) -o $@

test_image: test_image.o $(OBJS)
	$(CC) $(CFLAGS) test_image.o $(OBJS) -o $@

%.o: %.c image.h
	$(CC) $(CFLAGS) -c $< -o $@

images: imagegen
	./imagegen scene scene.bmp
	./imagegen mandelbrot mandelbrot.bmp
	./imagegen pattern pattern.bmp

test: test_image images
	./test_image
	test $$(wc -c < scene.bmp) -eq 768054

clean:
	rm -f *.o imagegen test_image *.bmp

.PHONY: images test clean
