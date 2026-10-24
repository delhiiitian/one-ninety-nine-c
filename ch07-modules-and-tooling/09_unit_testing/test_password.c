/*
 * Chapter 7.9 - Unit testing
 * Each TEST checks one behaviour. "make test" builds and runs them; exit code 0 = all passed.
 */
#include "mini_test.h"
#include "password.h"

TEST(short_passwords_are_weak)
{
    CHECK_EQ_INT(password_strength(""), WEAK);
    CHECK_EQ_INT(password_strength("Ab1!"), WEAK);           /* varied but too short */
}

TEST(long_but_plain_is_weak)
{
    CHECK_EQ_INT(password_strength("aaaaaaaaaa"), WEAK);
}

TEST(mixed_eight_chars_is_medium)
{
    CHECK_EQ_INT(password_strength("abcdEF12"), MEDIUM);
}

TEST(long_and_varied_is_strong)
{
    CHECK_EQ_INT(password_strength("correct-Horse-7-battery"), STRONG);
    CHECK_EQ_INT(password_strength("Tr0ub4dor&3"), STRONG);
}

TEST(names_are_readable)
{
    CHECK_EQ_STR(strength_name(WEAK), "weak");
    CHECK_EQ_STR(strength_name(STRONG), "strong");
    CHECK(strength_name(MEDIUM)[0] == 'm');
}

int main(void)
{
    RUN(short_passwords_are_weak);
    RUN(long_but_plain_is_weak);
    RUN(mixed_eight_chars_is_medium);
    RUN(long_and_varied_is_strong);
    RUN(names_are_readable);
    return TEST_REPORT();
}
