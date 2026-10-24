/*
 * Chapter 6.2 - Reading a file line by line with fgets
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    /* create a small shopping list to read back */
    FILE *f = fopen("shopping.txt", "w");
    if (f == NULL) { perror("shopping.txt"); return 1; }
    fputs("milk\n# comments start with a hash\neggs\n\nbread\nbananas\n", f);
    fclose(f);

    f = fopen("shopping.txt", "r");
    if (f == NULL) { perror("shopping.txt"); return 1; }

    char line[128];
    int number = 0, items = 0;
    while (fgets(line, sizeof line, f) != NULL) {   /* NULL at end of file */
        number++;
        line[strcspn(line, "\r\n")] = '\0';          /* remove the newline */
        if (line[0] == '\0' || line[0] == '#')
            continue;                                /* skip blanks and comments */
        items++;
        printf("line %d: item %d is %s\n", number, items, line);
    }
    if (ferror(f))                                   /* loop ended by an error, not EOF? */
        perror("reading shopping.txt");
    fclose(f);
    printf("%d items on %d lines\n", items, number);
    remove("shopping.txt");
    return 0;
}
