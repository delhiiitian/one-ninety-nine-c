/*
 * Chapter 6.1 - Opening and closing files
 * fopen gives you a FILE* handle; fclose hands it back.
 */
#include <stdio.h>

int main(void)
{
    /* "w" creates the file (or empties an existing one) for writing */
    FILE *f = fopen("diary.txt", "w");
    if (f == NULL) {                       /* always check */
        perror("diary.txt");
        return 1;
    }
    fprintf(f, "Dear diary, today I opened my first file in C.\n");
    if (fclose(f) != 0) {                  /* closing can fail too, e.g. disk full */
        perror("close diary.txt");
        return 1;
    }

    /* "a" appends to the end instead of erasing */
    f = fopen("diary.txt", "a");
    if (f == NULL) { perror("diary.txt"); return 1; }
    fprintf(f, "Later: I appended a second line.\n");
    fclose(f);

    /* "r" opens for reading; it fails if the file does not exist */
    f = fopen("diary.txt", "r");
    if (f == NULL) { perror("diary.txt"); return 1; }
    int ch, count = 0;
    while ((ch = fgetc(f)) != EOF) {       /* one character at a time */
        putchar(ch);
        count++;
    }
    fclose(f);
    printf("(%d characters)\n", count);

    if (fopen("does/not/exist.txt", "r") == NULL)
        perror("does/not/exist.txt");

    remove("diary.txt");                   /* delete the file */
    return 0;
}
