# Project 6: Word Counter

A small version of the Unix `wc` tool with an extra: the most frequent words. It reads any number of files, or standard input, in a single pass.

```
usage: wcount [-l] [-w] [-c] [-L] [-t N] [FILE...]
  -l lines   -w words   -c bytes   -L longest line   -t N  top N words
```

## Files

| File | Job |
| --- | --- |
| `counter.h` | `Counts`, `WordEntry`, `WordTable` and the API |
| `counter.c` | The one-pass counter and a hash table of word frequencies |
| `main.c` | Option parsing, looping over files, printing, exit status |
| `sample.txt` | Opening lines of *A Tale of Two Cities* |
| `test_counter.c` | Unit tests, including hash collisions |
| `Makefile` | `make`, `make test` (also compares with the system `wc`), `make clean` |

## Build and run

```sh
make
./wcount sample.txt
./wcount -t 5 sample.txt
cat *.c | ./wcount -l
make test
```
