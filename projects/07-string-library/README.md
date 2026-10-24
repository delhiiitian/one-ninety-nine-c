# Project 7: strkit, a tested string library

A small string library with a public header, three source files compiled into a static library (`libstrkit.a`), a unit-test suite and an example program. It shows how real C libraries are organised, built and tested.

## What is in it

| Group | Functions |
| --- | --- |
| Questions | `str_starts_with`, `str_ends_with`, `str_count`, `str_equals_ignore_case` |
| In place | `str_trim`, `str_to_upper`, `str_to_lower`, `str_reverse` |
| New strings (caller frees) | `str_dup`, `str_replace`, `str_join`, `str_split` + `str_free_list` |
| Builder | `sb_init`, `sb_append`, `sb_append_char`, `sb_appendf`, `sb_take`, `sb_free` |

## Layout

```
include/strkit.h      the public API: the only file users include
src/query.c           read-only functions
src/transform.c       in-place and allocating functions
src/builder.c         StrBuilder
tests/mini_test.h     a tiny test framework (from lesson 7.9)
tests/test_strkit.c   tests for every function
examples/slugify.c    an example program that uses the library
Makefile              build, test, asan, clean
```

## Build, test, use

```sh
make            # builds build/libstrkit.a and build/slugify
make test       # runs every unit test and the example
make asan       # the same tests under AddressSanitizer
./build/slugify "My Blog Post: Part 2!"
```

To use it in your own program:

```sh
gcc -Ipath/to/include myprog.c -Lpath/to/build -lstrkit -o myprog
```
