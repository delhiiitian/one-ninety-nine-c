/*
 * bmp.c - writes a 24-bit Windows BMP file by hand.
 * Every multi-byte number in a BMP is little-endian, so we write bytes ourselves
 * instead of dumping structs: the output is identical on every computer.
 */
#include <stdio.h>
#include "image.h"

static void put_u16(unsigned char *p, uint16_t v) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); }
static void put_u32(unsigned char *p, uint32_t v)
{
    p[0] = (unsigned char)v;
    p[1] = (unsigned char)(v >> 8);
    p[2] = (unsigned char)(v >> 16);
    p[3] = (unsigned char)(v >> 24);
}

static size_t row_size(int width)
{
    return ((size_t)width * 3 + 3) & ~(size_t)3;   /* each row padded to a multiple of 4 bytes */
}

size_t bmp_file_size(int width, int height)
{
    return 54 + row_size(width) * (size_t)height;
}

int bmp_write(const Image *img, const char *path)
{
    unsigned char header[54] = { 0 };
    size_t rs = row_size(img->width);
    uint32_t data_size = (uint32_t)(rs * img->height);

    header[0] = 'B';
    header[1] = 'M';
    put_u32(header + 2, 54 + data_size);      /* file size */
    put_u32(header + 10, 54);                 /* where pixel data starts */
    put_u32(header + 14, 40);                 /* info header size */
    put_u32(header + 18, (uint32_t)img->width);
    put_u32(header + 22, (uint32_t)img->height);   /* positive = rows stored bottom-up */
    put_u16(header + 26, 1);                  /* colour planes */
    put_u16(header + 28, 24);                 /* bits per pixel */
    put_u32(header + 34, data_size);
    put_u32(header + 38, 2835);               /* 72 DPI, in pixels per metre */
    put_u32(header + 42, 2835);

    FILE *f = fopen(path, "wb");
    if (f == NULL)
        return -1;
    int ok = fwrite(header, sizeof header, 1, f) == 1;

    unsigned char row[3 * 16384 + 4] = { 0 };
    for (int y = img->height - 1; ok && y >= 0; y--) {      /* bottom row first */
        for (int x = 0; x < img->width; x++) {
            Color c = image_get(img, x, y);
            row[x * 3 + 0] = BLUE_OF(c);                    /* BMP stores B, G, R */
            row[x * 3 + 1] = GREEN_OF(c);
            row[x * 3 + 2] = RED_OF(c);
        }
        ok = fwrite(row, 1, rs, f) == rs;
    }
    if (fclose(f) != 0)
        ok = 0;
    return ok ? 0 : -1;
}
