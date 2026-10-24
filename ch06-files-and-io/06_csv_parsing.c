/*
 * Chapter 6.6 - Parsing a CSV file into structs
 * Handles a header row, quoted fields with commas, and bad rows.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char city[32]; char country[32]; double temp_c; long population; } CityRow;

/* split one CSV line into at most max fields; supports "quoted, fields". Returns field count. */
int split_csv(char *line, char *fields[], int max)
{
    int n = 0;
    char *p = line;
    while (n < max) {
        if (*p == '"') {                       /* quoted field */
            fields[n++] = ++p;
            while (*p && *p != '"') p++;
            if (*p == '"') *p++ = '\0';
            if (*p == ',') p++; else break;
        } else {
            fields[n++] = p;
            p += strcspn(p, ",\r\n");
            if (*p == ',') *p++ = '\0';
            else { *p = '\0'; break; }
        }
    }
    return n;
}

int main(void)
{
    FILE *f = fopen("cities.csv", "w");
    if (f == NULL) { perror("cities.csv"); return 1; }
    fputs("city,country,avg_temp_c,population\n"
          "Delhi,India,25.0,32941000\n"
          "\"Washington, D.C.\",USA,14.5,689545\n"
          "Oslo,Norway,6.3,709037\n"
          "Broken row with no commas\n"
          "Lagos,Nigeria,27.1,15388000\n", f);
    fclose(f);

    f = fopen("cities.csv", "r");
    if (f == NULL) { perror("cities.csv"); return 1; }
    CityRow rows[16];
    int count = 0, line_no = 0;
    char line[256];
    while (fgets(line, sizeof line, f) && count < 16) {
        line_no++;
        if (line_no == 1)
            continue;                          /* skip the header */
        char *fields[4];
        if (split_csv(line, fields, 4) != 4) {
            fprintf(stderr, "line %d: expected 4 fields, skipping\n", line_no);
            continue;
        }
        CityRow *r = &rows[count++];
        snprintf(r->city, sizeof r->city, "%s", fields[0]);
        snprintf(r->country, sizeof r->country, "%s", fields[1]);
        r->temp_c = strtod(fields[2], NULL);
        r->population = strtol(fields[3], NULL, 10);
    }
    fclose(f);

    printf("%-18s %-8s %6s %12s\n", "City", "Country", "Temp", "Population");
    for (int i = 0; i < count; i++)
        printf("%-18s %-8s %6.1f %12ld\n", rows[i].city, rows[i].country, rows[i].temp_c, rows[i].population);
    remove("cities.csv");
    return 0;
}
