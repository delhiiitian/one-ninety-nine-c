/*
 * Chapter 5.7 - The "goto cleanup" pattern
 * When a function acquires several resources, one exit path releases them in reverse order.
 */
#include <stdio.h>
#include <stdlib.h>

/* copies a file while counting lines; returns 0 on success, -1 on failure */
int copy_with_line_count(const char *src_path, const char *dst_path, long *lines)
{
    int status = -1;            /* assume failure until the end */
    FILE *src = NULL, *dst = NULL;
    char *buffer = NULL;

    src = fopen(src_path, "r");
    if (src == NULL) { perror(src_path); goto cleanup; }

    dst = fopen(dst_path, "w");
    if (dst == NULL) { perror(dst_path); goto cleanup; }

    buffer = malloc(4096);
    if (buffer == NULL) { fprintf(stderr, "out of memory\n"); goto cleanup; }

    *lines = 0;
    while (fgets(buffer, 4096, src) != NULL) {
        if (fputs(buffer, dst) == EOF) { perror("write"); goto cleanup; }
        (*lines)++;
    }
    status = 0;                 /* only reached when everything worked */

cleanup:                        /* release in reverse order; each check skips what was never acquired */
    free(buffer);
    if (dst != NULL) fclose(dst);
    if (src != NULL) fclose(src);
    return status;
}

int main(void)
{
    FILE *f = fopen("goto_demo_in.txt", "w");
    if (f == NULL) { perror("create demo file"); return 1; }
    fprintf(f, "first line\nsecond line\nthird line\n");
    fclose(f);

    long lines = 0;
    if (copy_with_line_count("goto_demo_in.txt", "goto_demo_out.txt", &lines) == 0)
        printf("Copied %ld lines\n", lines);
    if (copy_with_line_count("missing.txt", "never_created.txt", &lines) != 0)
        printf("Second copy failed cleanly, nothing leaked\n");

    remove("goto_demo_in.txt");
    remove("goto_demo_out.txt");
    return 0;
}
