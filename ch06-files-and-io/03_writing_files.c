/*
 * Chapter 6.3 - Writing formatted files: a report generator
 */
#include <stdio.h>
#include <time.h>

typedef struct { const char *name; int units; double price; } Sale;

int main(void)
{
    Sale sales[] = {
        { "Notebook", 120, 2.50 }, { "Pen", 340, 0.80 },
        { "Backpack", 15, 34.99 }, { "Stapler", 42, 7.25 },
    };
    int n = sizeof sales / sizeof sales[0];

    FILE *out = fopen("sales_report.txt", "w");
    if (out == NULL) { perror("sales_report.txt"); return 1; }

    time_t now = time(NULL);
    char stamp[32];
    strftime(stamp, sizeof stamp, "%Y-%m-%d", localtime(&now));

    fprintf(out, "SALES REPORT  %s\n", stamp);
    fprintf(out, "%-10s %6s %8s %10s\n", "Item", "Units", "Price", "Revenue");
    double total = 0;
    for (int i = 0; i < n; i++) {
        double revenue = sales[i].units * sales[i].price;
        total += revenue;
        fprintf(out, "%-10s %6d %8.2f %10.2f\n", sales[i].name, sales[i].units,
                sales[i].price, revenue);
    }
    fprintf(out, "%-10s %26.2f\n", "TOTAL", total);

    if (fclose(out) != 0) { perror("sales_report.txt"); return 1; }

    /* show what we wrote */
    FILE *in = fopen("sales_report.txt", "r");
    if (in == NULL) { perror("sales_report.txt"); return 1; }
    char line[128];
    while (fgets(line, sizeof line, in))
        fputs(line, stdout);
    fclose(in);
    remove("sales_report.txt");
    return 0;
}
