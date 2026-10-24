#ifndef SETTINGS_H
#define SETTINGS_H

extern int verbose;          /* DECLARATION: "this variable exists in some .c file" */

void settings_set_volume(int v);
int  settings_volume(void);
int  settings_changes(void);

#endif
