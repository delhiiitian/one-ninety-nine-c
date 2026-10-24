#!/bin/sh
# Compiles and runs every chapter example and every project test.
# Usage: sh tests/run_all.sh        (from the repository root)
# An example reads tests/inputs/<chapter>/<name>.in as stdin when that file exists.
set -u
CC=${CC:-gcc}
CFLAGS="-std=c11 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L"
ROOT=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$ROOT/build"
mkdir -p "$BUILD"
pass=0; fail=0

for src in "$ROOT"/ch*/*.c; do
    chapter=$(basename "$(dirname "$src")")
    name=$(basename "$src" .c)
    exe="$BUILD/$chapter-$name"
    extra=""
    case "$chapter" in ch09*) extra="-pthread -lm" ;; esac
    if ! $CC $CFLAGS "$src" -o "$exe" $extra -lm 2>"$BUILD/err.txt"; then
        echo "FAIL (compile) $chapter/$name"; cat "$BUILD/err.txt"; fail=$((fail+1)); continue
    fi
    input="$ROOT/tests/inputs/$chapter/$name.in"
    [ -f "$input" ] || input=/dev/null
    if (cd "$BUILD" && timeout 10 "$exe" <"$input" >"$exe.out" 2>&1); then
        pass=$((pass+1))
    else
        code=$?
        expected="$ROOT/tests/inputs/$chapter/$name.exit"
        if [ -f "$expected" ] && [ "$(cat "$expected")" = "$code" ]; then
            pass=$((pass+1))
        else
            echo "FAIL (exit $code) $chapter/$name"; tail -5 "$exe.out"; fail=$((fail+1))
        fi
    fi
done

for proj in "$ROOT"/ch*/*/ "$ROOT"/projects/*/; do
    [ -f "$proj/Makefile" ] || continue
    if (cd "$proj" && make -s clean >/dev/null 2>&1; make -s test >"$BUILD/proj.out" 2>&1; s=$?; make -s clean >/dev/null 2>&1; exit $s); then
        pass=$((pass+1))
    else
        echo "FAIL project $(basename "$proj")"; tail -20 "$BUILD/proj.out"; fail=$((fail+1))
    fi
done

echo "passed: $pass  failed: $fail"
[ "$fail" -eq 0 ]
