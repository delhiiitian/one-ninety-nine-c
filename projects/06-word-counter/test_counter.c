/* test_counter.c - Run: make test */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "counter.h"

static FILE *from_text(const char *text)
{
    FILE *f = tmpfile();
    assert(f != NULL);
    fputs(text, f);
    rewind(f);
    return f;
}

int main(void)
{
    Counts c;
    FILE *f = from_text("hello world\nsecond  line here\n");
    assert(count_stream(f, &c, NULL) == 0);
    assert(c.lines == 2 && c.words == 5 && c.bytes == 30 && c.longest_line == 17);
    fclose(f);

    f = from_text("no newline at end");
    count_stream(f, &c, NULL);
    assert(c.lines == 0 && c.words == 4 && c.longest_line == 17);
    fclose(f);

    f = from_text("");
    count_stream(f, &c, NULL);
    assert(c.lines == 0 && c.words == 0 && c.bytes == 0);
    fclose(f);

    WordTable t;
    assert(table_init(&t, 8) == 0);           /* tiny table forces collisions */
    f = from_text("The cat and the hat. THE end, and the cat!");
    assert(count_stream(f, &c, &t) == 0);
    fclose(f);
    WordEntry *top[3];
    assert(table_top(&t, top, 3) == 3);
    assert(strcmp(top[0]->word, "the") == 0 && top[0]->count == 4);
    assert(strcmp(top[1]->word, "and") == 0 && top[1]->count == 2);
    assert(strcmp(top[2]->word, "cat") == 0 && top[2]->count == 2);
    assert(t.unique == 5);
    table_free(&t);

    Counts total = { 0, 0, 0, 0 }, a = { 1, 2, 3, 4 }, b = { 10, 20, 30, 2 };
    counts_add(&total, &a);
    counts_add(&total, &b);
    assert(total.lines == 11 && total.words == 22 && total.bytes == 33 && total.longest_line == 4);
    printf("All counter tests passed.\n");
    return 0;
}
