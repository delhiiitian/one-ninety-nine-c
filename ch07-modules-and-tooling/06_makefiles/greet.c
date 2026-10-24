#include <stdio.h>
#include "greet.h"

void greet(const char *name, int hour)
{
    const char *part = hour < 12 ? "morning" : hour < 18 ? "afternoon" : "evening";
    printf("Good %s, %s!\n", part, name);
}
