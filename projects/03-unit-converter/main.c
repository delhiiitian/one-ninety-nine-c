/*
 * Project 3 - Unit Converter
 * Interactive:  ./convert          then type lines like "5 km mi"
 * One-shot:     ./convert 100 F C
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "units.h"

static void convert_and_print(double value, const char *from_name, const char *to_name)
{
    const Unit *from = find_unit(from_name);
    const Unit *to = find_unit(to_name);
    double result;

    if (from == NULL || to == NULL) {
        printf("  Unknown unit '%s'. Type 'list' to see units.\n", from == NULL ? from_name : to_name);
        return;
    }
    if (convert(value, from, to, &result) != 0) {
        printf("  Cannot convert %s (%s) to %s (%s).\n", from->long_name,
               category_name(from->category), to->long_name, category_name(to->category));
        return;
    }
    printf("  %g %s = %.4g %s\n", value, from->long_name, result, to->long_name);
}

int main(int argc, char *argv[])
{
    if (argc == 4) {                                 /* one-shot mode */
        convert_and_print(atof(argv[1]), argv[2], argv[3]);
        return 0;
    }

    printf("=== Unit Converter ===\nType: <value> <from> <to>   e.g. 5 km mi\n");
    printf("Or 'list' to see units, 'quit' to exit.\n");
    char line[128];
    for (;;) {
        printf("> ");
        if (fgets(line, sizeof line, stdin) == NULL)
            break;                                   /* end of input */
        if (strncmp(line, "quit", 4) == 0)
            break;
        if (strncmp(line, "list", 4) == 0) {
            list_units();
            continue;
        }
        double value;
        char from[16], to[16];
        if (sscanf(line, "%lf %15s %15s", &value, from, to) == 3)
            convert_and_print(value, from, to);
        else if (line[0] != '\n')
            printf("  Please type a value and two units, e.g. 70 kg lb\n");
    }
    printf("Bye!\n");
    return 0;
}
