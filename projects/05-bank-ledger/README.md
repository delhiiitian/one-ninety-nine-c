# Project 5: Bank Ledger

A command-driven ledger that processes deposits, withdrawals and transfers from a file or from the keyboard. Its job is to be impossible to break: every input is validated, every failure has a named error code, a failed command changes nothing, and the exit code tells scripts whether every line succeeded.

## Commands

```
open NAME [AMOUNT]
deposit NAME AMOUNT
withdraw NAME AMOUNT
transfer FROM TO AMOUNT
balance NAME
# comments are ignored
```

Amounts are `12`, `12.5` or `12.50`. Negative amounts, three decimals and junk are rejected.

## Files

| File | Job |
| --- | --- |
| `ledger.h` | `LedgerError`, `Account`, `Ledger` and the API |
| `ledger.c` | Strict amount parsing and all account operations; never prints or exits |
| `main.c` | Reads lines, reports errors with line numbers, prints the report, sets the exit code |
| `sample.txt` | A sample day with six deliberate mistakes |
| `test_ledger.c` | Unit tests for parsing and every error path |
| `Makefile` | `make`, `make test`, `make asan` (sanitizer build), `make clean` |

## Build and run

```sh
make
./ledger sample.txt ; echo "exit code: $?"
./ledger                 # type commands, Ctrl+D to finish
make test
make asan                # same tests under AddressSanitizer
```
