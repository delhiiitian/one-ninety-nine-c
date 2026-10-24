# Project 3: Unit Converter

Converts length, weight, volume and temperature units. Every unit is one row in a table, so adding a new unit is a one-line change.

## Files

| File | Job |
| --- | --- |
| `units.h` | The `Unit` type, the `Category` enum and the public functions |
| `units.c` | The unit table and the conversion maths |
| `main.c` | One-shot mode (`./convert 5 km mi`) and interactive mode |
| `test_units.c` | Checks conversions against known values |
| `Makefile` | `make`, `make test`, `make clean` |

## Build and run

```sh
make
./convert 100 F C       # one conversion
./convert               # interactive: type "5 km mi", "list" or "quit"
make test
```

## Ideas to extend it

- Add a speed category (km/h, mph, m/s).
- Add currency with rates read from a file.
