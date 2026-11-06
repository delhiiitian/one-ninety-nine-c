"""Chapter 9.9 - calling C from Python with ctypes.

Run after building the library:  make && python3 use_fastmath.py
"""
import ctypes
import pathlib
import time

lib = ctypes.CDLL(str(pathlib.Path(__file__).with_name("libfastmath.so")))

# tell ctypes the C signatures, or it assumes everything is int
lib.count_primes.argtypes = [ctypes.c_long]
lib.count_primes.restype = ctypes.c_long
lib.mean.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_size_t]
lib.mean.restype = ctypes.c_double
lib.to_upper_copy.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t]
lib.to_upper_copy.restype = ctypes.c_size_t


def count_primes_python(n):
    composite = bytearray(n)
    count = 0
    for i in range(2, n):
        if not composite[i]:
            count += 1
            for j in range(i * i, n, i):
                composite[j] = 1
    return count


N = 2_000_000
t0 = time.perf_counter()
py = count_primes_python(N)
t1 = time.perf_counter()
c = lib.count_primes(N)
t2 = time.perf_counter()
print(f"primes below {N:,}: Python {py} in {t1 - t0:.3f}s, C {c} in {t2 - t1:.3f}s")
print(f"C was about {(t1 - t0) / max(t2 - t1, 1e-9):.0f}x faster")

values = [72.5, 88.0, 91.5, 64.0]
array = (ctypes.c_double * len(values))(*values)      # a C array built from a Python list
print("mean from C:", lib.mean(array, len(values)))

out = ctypes.create_string_buffer(32)                 # writable memory for C to fill
n = lib.to_upper_copy(b"hello from python", out, len(out))
print("C wrote", n, "bytes:", out.value.decode())
