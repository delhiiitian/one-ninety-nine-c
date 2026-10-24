/*
 * Chapter 6.5 - Seeking: jumping to any position in a file
 * fseek moves the position, ftell reports it, rewind goes back to the start.
 */
#include <stdio.h>

typedef struct { int id; char title[28]; } Record;   /* fixed-size records: 32 bytes */

int main(void)
{
    FILE *f = fopen("movies.db", "w+b");              /* read AND write */
    if (f == NULL) { perror("movies.db"); return 1; }

    const char *titles[] = { "Inception", "Up", "Coco", "Arrival", "Dune" };
    for (int i = 0; i < 5; i++) {
        Record r = { i + 1, "" };
        snprintf(r.title, sizeof r.title, "%s", titles[i]);
        fwrite(&r, sizeof r, 1, f);
    }

    /* file size: jump to the end and ask where we are */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    printf("File is %ld bytes = %ld records\n", size, size / (long)sizeof(Record));

    /* read record #4 directly, without reading 1-3 (random access) */
    Record r;
    fseek(f, 3 * (long)sizeof(Record), SEEK_SET);
    if (fread(&r, sizeof r, 1, f) == 1)
        printf("Record 4: id=%d title=%s\n", r.id, r.title);

    /* overwrite record #2 in place */
    Record fixed = { 2, "Up (2009)" };
    fseek(f, 1 * (long)sizeof(Record), SEEK_SET);
    fwrite(&fixed, sizeof fixed, 1, f);

    /* read everything again from the start */
    rewind(f);
    while (fread(&r, sizeof r, 1, f) == 1)
        printf("  %d: %s\n", r.id, r.title);

    /* read the LAST record by seeking backwards from the end */
    fseek(f, -(long)sizeof(Record), SEEK_END);
    if (fread(&r, sizeof r, 1, f) == 1)
        printf("Last record: %s\n", r.title);

    fclose(f);
    remove("movies.db");
    return 0;
}
