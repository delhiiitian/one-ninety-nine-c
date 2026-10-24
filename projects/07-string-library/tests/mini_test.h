/*
 * mini_test.h - a tiny unit-testing framework in one header (about 30 lines)
 * Usage:
 *   TEST(name) { CHECK(cond); CHECK_EQ_INT(a, b); }
 *   int main(void) { RUN(name); return TEST_REPORT(); }
 */
#ifndef MINI_TEST_H
#define MINI_TEST_H

#include <stdio.h>
#include <string.h>

static int mt_checks = 0, mt_failures = 0;

#define TEST(name) static void name(void)

#define CHECK(cond) do { \
        mt_checks++; \
        if (!(cond)) { \
            mt_failures++; \
            printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        } \
    } while (0)

#define CHECK_EQ_INT(actual, expected) do { \
        long mt_a = (long)(actual), mt_e = (long)(expected); \
        mt_checks++; \
        if (mt_a != mt_e) { \
            mt_failures++; \
            printf("  FAIL %s:%d: %s == %ld, expected %ld\n", __FILE__, __LINE__, #actual, mt_a, mt_e); \
        } \
    } while (0)

#define CHECK_EQ_STR(actual, expected) do { \
        const char *mt_a = (actual), *mt_e = (expected); \
        mt_checks++; \
        if (strcmp(mt_a, mt_e) != 0) { \
            mt_failures++; \
            printf("  FAIL %s:%d: %s == \"%s\", expected \"%s\"\n", __FILE__, __LINE__, #actual, mt_a, mt_e); \
        } \
    } while (0)

#define RUN(name) do { printf("running %s\n", #name); name(); } while (0)

#define TEST_REPORT() \
    (printf("%d checks, %d failures\n", mt_checks, mt_failures), mt_failures == 0 ? 0 : 1)

#endif
