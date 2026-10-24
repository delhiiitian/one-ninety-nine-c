#include "settings.h"

int verbose = 0;             /* DEFINITION: the one real variable, shared via extern */

static int volume = 5;       /* static: private to this file, no other file can touch it */
static int change_count = 0;

static int clamp(int v, int lo, int hi)   /* static function: a private helper */
{
    return v < lo ? lo : v > hi ? hi : v;
}

void settings_set_volume(int v)
{
    volume = clamp(v, 0, 10); /* the only way in is through this checked function */
    change_count++;
}

int settings_volume(void)  { return volume; }
int settings_changes(void) { return change_count; }
