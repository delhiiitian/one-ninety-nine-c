# Project 1: Bill Splitter

A command-line tool that splits a restaurant bill, with tax and tip, between friends. Leftover cents go to the first people in the list, so the shares always add up to the exact total.

## Files

| File | Job |
| --- | --- |
| `bill.h` | The `Bill` and `Split` types and the two function prototypes |
| `bill.c` | The money maths. No printing, so it is easy to test |
| `main.c` | Asks the questions and prints the receipt |
| `test_bill.c` | Checks the maths against known answers |
| `Makefile` | Builds the program and runs the tests |

## Build and run

```sh
make            # builds ./bill_splitter
./bill_splitter
make test       # runs the unit tests and a sample session
```

## Sample session

```
=== Bill Splitter ===
Bill amount before tax ($): 86.40
Tax percent (e.g. 8): 8
Tip percent (e.g. 15): 18
Number of people: 3
```

## Ideas to extend it

- Let each person enter what they ordered and pay their own share.
- Round each share up to the nearest dollar and show the extra tip.
