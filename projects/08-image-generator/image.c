/* image.c - pixels and shapes */
#include <stdlib.h>
#include "image.h"

int image_create(Image *img, int width, int height)
{
    if (width <= 0 || height <= 0 || width > 16384 || height > 16384)
        return -1;
    img->pixels = calloc((size_t)width * (size_t)height, sizeof *img->pixels);
    if (img->pixels == NULL)
        return -1;
    img->width = width;
    img->height = height;
    return 0;
}

void image_free(Image *img)
{
    free(img->pixels);
    img->pixels = NULL;
    img->width = img->height = 0;
}

Color image_get(const Image *img, int x, int y)
{
    if (x < 0 || y < 0 || x >= img->width || y >= img->height)
        return 0;
    return img->pixels[(size_t)y * img->width + x];
}

void image_set(Image *img, int x, int y, Color c)
{
    if (x < 0 || y < 0 || x >= img->width || y >= img->height)
        return;                                   /* clipping: drawing off-canvas is safe */
    img->pixels[(size_t)y * img->width + x] = c;  /* 2D position -> 1D index */
}

void draw_fill(Image *img, Color c)
{
    size_t n = (size_t)img->width * img->height;
    for (size_t i = 0; i < n; i++)
        img->pixels[i] = c;
}

void draw_rect(Image *img, int x, int y, int w, int h, Color c)
{
    for (int j = y; j < y + h; j++)
        for (int i = x; i < x + w; i++)
            image_set(img, i, j, c);
}

void draw_circle(Image *img, int cx, int cy, int r, Color c)
{
    for (int y = -r; y <= r; y++)
        for (int x = -r; x <= r; x++)
            if (x * x + y * y <= r * r)           /* inside the circle: no sqrt needed */
                image_set(img, cx + x, cy + y, c);
}

/* Bresenham's line algorithm: integers only */
void draw_line(Image *img, int x0, int y0, int x1, int y1, Color c)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        image_set(img, x0, y0, c);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

/* mix two colours channel by channel; t256 = 0 gives a, 256 gives b */
Color blend(Color a, Color b, int t256)
{
    int r = RED_OF(a)   + ((RED_OF(b)   - RED_OF(a))   * t256 >> 8);
    int g = GREEN_OF(a) + ((GREEN_OF(b) - GREEN_OF(a)) * t256 >> 8);
    int bl = BLUE_OF(a) + ((BLUE_OF(b)  - BLUE_OF(a))  * t256 >> 8);
    return RGB(r, g, bl);
}

void draw_vertical_gradient(Image *img, Color top, Color bottom)
{
    for (int y = 0; y < img->height; y++) {
        Color row = blend(top, bottom, img->height > 1 ? y * 256 / (img->height - 1) : 0);
        for (int x = 0; x < img->width; x++)
            img->pixels[(size_t)y * img->width + x] = row;
    }
}
