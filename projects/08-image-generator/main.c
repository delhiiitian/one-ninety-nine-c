/*
 * Project 8 - Image Generator
 *   ./imagegen scene      [out.bmp]   a sunset landscape drawn with shapes
 *   ./imagegen mandelbrot [out.bmp]   the Mandelbrot fractal
 *   ./imagegen pattern    [out.bmp]   an XOR bit pattern
 * Open the .bmp in any image viewer or browser.
 */
#include <stdio.h>
#include <string.h>
#include "image.h"

static void draw_scene(Image *img)
{
    int w = img->width, h = img->height;
    draw_vertical_gradient(img, RGB(255, 94, 77), RGB(255, 200, 120));    /* sunset sky */
    draw_circle(img, w / 2, h * 6 / 10, h / 6, RGB(255, 236, 140));     /* sun */
    draw_rect(img, 0, h * 6 / 10, w, h, RGB(30, 60, 110));               /* sea */
    for (int i = 0; i < 12; i++) {                                       /* light on water */
        int y = h * 6 / 10 + 8 + i * 9;
        int half = (w / 5) - i * 6;
        if (half > 0)
            draw_line(img, w / 2 - half, y, w / 2 + half, y, RGB(255, 200, 140));
    }
    for (int i = 0; i < 3; i++) {                                        /* hills */
        int cx = w / 6 + i * w / 3;
        draw_circle(img, cx, h * 6 / 10 + h / 6, h / 4, RGB(20 + i * 10, 70 + i * 15, 40));
    }
    draw_rect(img, 0, h * 9 / 10, w, h / 10, RGB(10, 40, 25));           /* foreground */
}

static void draw_pattern(Image *img)
{
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++) {
            int v = (x ^ y) & 0xFF;                                      /* XOR texture */
            image_set(img, x, y, RGB(v, (v * 3) & 0xFF, 255 - v));
        }
}

int main(int argc, char *argv[])
{
    const char *mode = argc > 1 ? argv[1] : "scene";
    char default_path[64];
    snprintf(default_path, sizeof default_path, "%s.bmp", mode);
    const char *path = argc > 2 ? argv[2] : default_path;

    Image img;
    if (image_create(&img, 640, 400) != 0) {
        fprintf(stderr, "could not allocate image\n");
        return 1;
    }

    if (strcmp(mode, "scene") == 0)
        draw_scene(&img);
    else if (strcmp(mode, "mandelbrot") == 0)
        render_mandelbrot(&img, -0.6, 0.0, 1.0, 200);
    else if (strcmp(mode, "pattern") == 0)
        draw_pattern(&img);
    else {
        fprintf(stderr, "usage: %s [scene|mandelbrot|pattern] [out.bmp]\n", argv[0]);
        image_free(&img);
        return 2;
    }

    if (bmp_write(&img, path) != 0) {
        perror(path);
        image_free(&img);
        return 1;
    }
    printf("wrote %s (%dx%d, %zu bytes)\n", path, img.width, img.height, bmp_file_size(img.width, img.height));
    image_free(&img);
    return 0;
}
