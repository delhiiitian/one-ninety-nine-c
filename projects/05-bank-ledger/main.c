/*
 * Project 5 - Bank Ledger
 * Reads commands from a file or stdin, one per line:
 *   open NAME [AMOUNT]   deposit NAME AMOUNT   withdraw NAME AMOUNT
 *   transfer FROM TO AMOUNT   balance NAME   # comment
 * Usage: ./ledger [file]      Exit code: 0 all ok, 1 some lines failed, 2 bad usage
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ledger.h"

static void print_report(const Ledger *l)
{
    long total = 0;
    printf("\n%-20s %15s %6s\n", "Account", "Balance", "Txns");
    for (size_t i = 0; i < l->count; i++) {
        const Account *a = &l->accounts[i];
        printf("%-20s %12ld.%02ld %6d\n", a->name, a->balance_cents / 100,
               a->balance_cents % 100, a->transactions);
        total += a->balance_cents;
    }
    printf("%-20s %12ld.%02ld\n", "TOTAL", total / 100, total % 100);
}

int main(int argc, char *argv[])
{
    if (argc > 2) {
        fprintf(stderr, "usage: %s [commands-file]\n", argv[0]);
        return 2;
    }
    FILE *in = stdin;
    if (argc == 2) {
        in = fopen(argv[1], "r");
        if (in == NULL) {
            perror(argv[1]);
            return 2;
        }
    }

    static Ledger ledger;            /* static: zero-initialised, and too big for a small stack */
    char line[256], reply[128];
    int line_no = 0, failures = 0;

    while (fgets(line, sizeof line, in) != NULL) {
        line_no++;
        if (strchr(line, '\n') == NULL && !feof(in)) {     /* line longer than the buffer */
            fprintf(stderr, "line %d: too long, skipped\n", line_no);
            int ch;
            while ((ch = fgetc(in)) != '\n' && ch != EOF)
                ;
            failures++;
            continue;
        }
        char *hash = strchr(line, '#');
        if (hash != NULL)
            *hash = '\0';                                  /* strip comments */

        LedgerError e = ledger_execute(&ledger, line, reply, sizeof reply);
        if (e != LEDGER_OK) {
            size_t len = strcspn(line, "\r\n");
            while (len > 0 && line[len - 1] == ' ')
                len--;                                     /* trim trailing spaces */
            line[len] = '\0';
            fprintf(stderr, "line %d: %s  (\"%s\")\n", line_no, ledger_error_message(e), line);
            failures++;
        } else if (reply[0] != '\0') {
            printf("line %d: %s\n", line_no, reply);
        }
    }
    if (ferror(in)) {
        perror("read");
        failures++;
    }
    if (in != stdin)
        fclose(in);

    print_report(&ledger);
    printf("%d line(s) read, %d failed\n", line_no, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
