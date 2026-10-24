/* test_image.c - Run: make test */
#include <assert.h>
#include <stdio.h>
#include "image.h"

int main(void)
{
    assert(RGB(255, 128, 1) == 0xFF8001);
    assert(RED_OF(0xFF8001) == 255 && GREEN_OF(0xFF8001) == 128 && BLUE_OF(0xFF8001) == 1);
    assert(blend(RGB(0, 0, 0), RGB(200, 100, 50), 128) == RGB(100, 50, 25));
    assert(blend(RGB(10, 20, 30), RGB(200, 100, 50), 0) == RGB(10, 20, 30));

    Image img;
    assert(image_create(&img, 0, 10) == -1);
    assert(image_create(&img, 5, 3) == 0);
    image_set(&img, 4, 2, RGB(1, 2, 3));
    image_set(&img, 99, 99, RGB(9, 9, 9));          /* off canvas: ignored, no crash */
    assert(image_get(&img, 4, 2) == RGB(1, 2, 3));
    draw_rect(&img, -2, -2, 4, 4, RGB(7, 7, 7));    /* partly off canvas */
    assert(image_get(&img, 1, 1) == RGB(7, 7, 7) && image_get(&img, 2, 2) == 0);
    draw_line(&img, 0, 2, 4, 0, RGB(5, 5, 5));
    assert(image_get(&img, 0, 2) == RGB(5, 5, 5) && image_get(&img, 4, 0) == RGB(5, 5, 5));

    /* BMP: 5 px * 3 bytes = 15, padded to 16 per row; 54 + 16*3 = 102 bytes */
    assert(bmp_file_size(5, 3) == 102);
    assert(bmp_write(&img, "test.bmp") == 0);
    FILE *f = fopen("test.bmp", "rb");
    assert(f != NULL);
    unsigned char h[54];
    assert(fread(h, 1, 54, f) == 54);
    fseek(f, 0, SEEK_END);
    assert(ftell(f) == 102);
    fclose(f);
    remove("test.bmp");
    assert(h[0] == 'B' && h[1] == 'M');
    assert(h[2] == 102 && h[3] == 0);               /* size, little-endian */
    assert(h[18] == 5 && h[22] == 3 && h[28] == 24);
    image_free(&img);

    printf("All image tests passed.\n");
    return 0;
}
