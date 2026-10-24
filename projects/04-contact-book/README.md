# Project 4: Contact Book

An interactive address book that stores contacts in a growable array in memory. Add, list (sorted by name), search by any part of a name, phone or email, and delete.

## Files

| File | Job |
| --- | --- |
| `contacts.h` | `Contact`, `ContactBook` and `Group` types and the API |
| `contacts.c` | Growable array, case-insensitive find and search, delete, sort |
| `main.c` | The command loop: add, list, search, delete, quit |
| `test_contacts.c` | Tests growth, duplicates, delete, sort and search |
| `Makefile` | `make`, `make test`, `make clean` |

## Build and run

```sh
make
./contacts
make test
```

## Ideas to extend it

- Save and load the book from a CSV file (Chapter 6).
- Add an `edit` command that changes a phone number.
