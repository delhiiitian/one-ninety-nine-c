/* test_strkit.c - unit tests for every strkit function. Run: make test */
#include <stdlib.h>
#include "mini_test.h"
#include "strkit.h"

TEST(queries)
{
    CHECK(str_starts_with("hello.c", "hello"));
    CHECK(!str_starts_with("he", "hello"));
    CHECK(str_ends_with("report.pdf", ".pdf"));
    CHECK(!str_ends_with("pdf", "report.pdf"));
    CHECK(str_ends_with("abc", ""));
    CHECK_EQ_INT(str_count("banana", "an"), 2);
    CHECK_EQ_INT(str_count("aaaa", "aa"), 2);             /* non-overlapping */
    CHECK_EQ_INT(str_count("abc", ""), 0);
    CHECK(str_equals_ignore_case("Delhi", "dELHI"));
    CHECK(!str_equals_ignore_case("Delhi", "Delhi!"));
}

TEST(in_place)
{
    char a[] = "   padded text \t\n";
    CHECK_EQ_STR(str_trim(a), "padded text");
    char b[] = "    ";
    CHECK_EQ_STR(str_trim(b), "");
    char c[] = "MiXeD 123";
    CHECK_EQ_STR(str_to_upper(c), "MIXED 123");
    CHECK_EQ_STR(str_to_lower(c), "mixed 123");
    char d[] = "stressed";
    CHECK_EQ_STR(str_reverse(d), "desserts");
    char e[] = "";
    CHECK_EQ_STR(str_reverse(e), "");
}

TEST(replace)
{
    char *r = str_replace("the cat sat on the mat", "the", "a");
    CHECK_EQ_STR(r, "a cat sat on a mat");
    free(r);
    r = str_replace("aaa", "a", "bb");
    CHECK_EQ_STR(r, "bbbbbb");
    free(r);
    r = str_replace("no match here", "xyz", "!");
    CHECK_EQ_STR(r, "no match here");
    free(r);
    r = str_replace("remove-the-dashes", "-", "");
    CHECK_EQ_STR(r, "removethedashes");
    free(r);
}

TEST(split_and_join)
{
    size_t n = 0;
    char **parts = str_split("red,green,,blue", ',', &n);
    CHECK_EQ_INT(n, 4);
    CHECK_EQ_STR(parts[0], "red");
    CHECK_EQ_STR(parts[2], "");                            /* empty field kept */
    CHECK_EQ_STR(parts[3], "blue");
    char *joined = str_join((const char *const *)parts, n, " | ");
    CHECK_EQ_STR(joined, "red | green |  | blue");
    free(joined);
    str_free_list(parts, n);

    parts = str_split("single", ',', &n);
    CHECK_EQ_INT(n, 1);
    CHECK_EQ_STR(parts[0], "single");
    str_free_list(parts, n);

    char *empty = str_join(NULL, 0, ",");
    CHECK_EQ_STR(empty, "");
    free(empty);
}

TEST(builder)
{
    StrBuilder sb;
    sb_init(&sb);
    for (int i = 0; i < 100; i++)                         /* forces several resizes */
        CHECK(sb_append_char(&sb, (char)('a' + i % 26)));
    CHECK_EQ_INT(sb.length, 100);
    sb_free(&sb);

    sb_append(&sb, "Total: ");
    sb_appendf(&sb, "%d items, $%.2f", 3, 19.5);
    CHECK_EQ_STR(sb.data, "Total: 3 items, $19.50");
    char *owned = sb_take(&sb);
    CHECK(sb.data == NULL && sb.length == 0);
    CHECK_EQ_STR(owned, "Total: 3 items, $19.50");
    free(owned);
}

int main(void)
{
    RUN(queries);
    RUN(in_place);
    RUN(replace);
    RUN(split_and_join);
    RUN(builder);
    return TEST_REPORT();
}
