/* fractal.c - the Mandelbrot set, coloured by how fast each point escapes */
#include "image.h"

static Color palette(int iter, int max_iter)
{
    if (iter >= max_iter)
        return RGB(0, 0, 0);                       /* inside the set: black */
    int t = iter * 256 / max_iter * 4 % 256;       /* repeat the colour ramp 4 times */
    static const Color stops[] = { 0x000764, 0x206BCB, 0xEDFFFF, 0xFFAA00, 0x000200 };
    int band = (iter * 4 / max_iter) % 4;
    return blend(stops[band], stops[band + 1], t);
}

void render_mandelbrot(Image *img, double cx, double cy, double zoom, int max_iter)
{
    double scale = 3.0 / (zoom * img->width);
    for (int py = 0; py < img->height; py++) {
        for (int px = 0; px < img->width; px++) {
            double x0 = cx + (px - img->width / 2.0) * scale;
            double y0 = cy + (py - img->height / 2.0) * scale;
            double x = 0, y = 0;
            int iter = 0;
            while (x * x + y * y <= 4.0 && iter < max_iter) {   /* z = z^2 + c */
                double xt = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xt;
                iter++;
            }
            image_set(img, px, py, palette(iter, max_iter));
        }
    }
}
