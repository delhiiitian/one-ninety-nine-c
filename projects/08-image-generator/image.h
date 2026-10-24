/* image.h - an in-memory RGB image, drawing functions and a BMP writer */
#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t Color;                 /* 0x00RRGGBB packed into 32 bits */

#define RGB(r, g, b) ((Color)(((uint32_t)(r) & 0xFF) << 16 | ((uint32_t)(g) & 0xFF) << 8 | ((uint32_t)(b) & 0xFF)))
#define RED_OF(c)   ((uint8_t)((c) >> 16))
#define GREEN_OF(c) ((uint8_t)((c) >> 8))
#define BLUE_OF(c)  ((uint8_t)(c))

typedef struct {
    int    width, height;
    Color *pixels;                      /* width * height, row by row, top row first */
} Image;

int   image_create(Image *img, int width, int height);
void  image_free(Image *img);
Color image_get(const Image *img, int x, int y);
void  image_set(Image *img, int x, int y, Color c);    /* ignores points outside */

void  draw_fill(Image *img, Color c);
void  draw_rect(Image *img, int x, int y, int w, int h, Color c);
void  draw_circle(Image *img, int cx, int cy, int r, Color c);
void  draw_line(Image *img, int x0, int y0, int x1, int y1, Color c);
void  draw_vertical_gradient(Image *img, Color top, Color bottom);
Color blend(Color a, Color b, int t256);                /* t256 = 0..256 */

void  render_mandelbrot(Image *img, double cx, double cy, double zoom, int max_iter);

int   bmp_write(const Image *img, const char *path);   /* 0 ok, -1 error (errno set) */
size_t bmp_file_size(int width, int height);

#endif
