# Project 8: Image Generator

Draws pictures pixel by pixel in memory and saves them as 24-bit BMP files, with no libraries at all. It can paint a sunset landscape from shapes, render the Mandelbrot fractal, or make an XOR bit pattern.

## Files

| File | Job |
| --- | --- |
| `image.h` | `Image` and `Color` types, colour macros, all functions |
| `image.c` | Pixels, clipping, rectangles, circles, Bresenham lines, gradients, blending |
| `fractal.c` | The Mandelbrot set with a colour palette |
| `bmp.c` | A hand-written BMP encoder (little-endian bytes, padded rows) |
| `main.c` | Picks a mode and writes the file |
| `test_image.c` | Tests colours, clipping, lines and the exact BMP header bytes |
| `Makefile` | `make`, `make images`, `make test`, `make clean` |

## Build and run

```sh
make
./imagegen scene          # writes scene.bmp
./imagegen mandelbrot     # writes mandelbrot.bmp
./imagegen pattern my.bmp
make test
```

Open the `.bmp` files in any image viewer or drag them into a browser. To convert to PNG: `convert scene.bmp scene.png` (ImageMagick).
