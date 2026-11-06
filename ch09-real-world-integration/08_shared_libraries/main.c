/*
 * Chapter 9.8 - Shared libraries (.so / .dylib / .dll)
 * 1) Link at build time:   gcc main.c -L. -ltax -o app  (run with LD_LIBRARY_PATH=. ./app)
 * 2) Load at run time:     dlopen("./libtax_uk.so") picks a "plugin" while running
 */
#define _POSIX_C_SOURCE 200809L
#include <dlfcn.h>
#include <stdio.h>
#include "tax.h"

typedef double (*TaxFn)(double);
typedef const char *(*RegionFn)(void);

int main(int argc, char *argv[])
{
    double bill = 250.0;
    printf("[linked]  %-14s tax on %.2f = %.2f\n", tax_region(), bill, tax_for(bill));

    const char *plugin = argc > 1 ? argv[1] : "./libtax_uk.so";
    void *lib = dlopen(plugin, RTLD_NOW);              /* load a library while running */
    if (lib == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        return 1;
    }
    TaxFn tax = (TaxFn)dlsym(lib, "tax_for");          /* look a function up by name */
    RegionFn region = (RegionFn)dlsym(lib, "tax_region");
    if (tax == NULL || region == NULL) {
        fprintf(stderr, "dlsym: %s\n", dlerror());
        dlclose(lib);
        return 1;
    }
    printf("[plugin]  %-14s tax on %.2f = %.2f\n", region(), bill, tax(bill));
    dlclose(lib);
    return 0;
}
