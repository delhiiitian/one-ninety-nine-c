/*
 * Chapter 6.9 - Buffering: why output sometimes appears late
 * stdio collects output in a buffer and writes it in big chunks.
 */
#include <stdio.h>
#include <time.h>

static double seconds_since(struct timespec start)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1e9;
}

static double write_lines(const char *path, int mode, size_t size)
{
    FILE *f = fopen(path, "w");
    if (f == NULL) { perror(path); return -1; }
    setvbuf(f, NULL, mode, size);          /* choose the buffering BEFORE the first write */
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < 100000; i++)
        fprintf(f, "log line %d\n", i);
    fclose(f);
    remove(path);
    return seconds_since(start);
}

int main(void)
{
    printf("Writing 100000 lines with different buffering:\n");
    printf("  fully buffered (64 KB): %.3f s\n", write_lines("buf_full.txt", _IOFBF, 65536));
    printf("  line buffered:          %.3f s\n", write_lines("buf_line.txt", _IOLBF, 1024));
    printf("  unbuffered:             %.3f s\n", write_lines("buf_none.txt", _IONBF, 0));

    /* a prompt without \n may sit in the buffer; fflush pushes it out now */
    printf("Progress: ");
    fflush(stdout);
    for (int i = 0; i < 5; i++) {
        printf("#");
        fflush(stdout);                    /* show each step immediately */
    }
    printf(" done\n");

    fprintf(stderr, "stderr is unbuffered, so errors appear at once\n");
    return 0;
}
