/*
 * Chapter 6.4 - Binary files: saving structs with fwrite and fread
 */
#include <stdio.h>
#include <string.h>

typedef struct {
    char   name[16];
    int    level;
    int    gold;
    double play_hours;
} SaveGame;

int main(void)
{
    SaveGame slots[3] = {
        { "Aria",  12, 1500, 10.5 },
        { "Bram",   3,   90,  1.2 },
        { "Cyra",  27, 9900, 48.0 },
    };

    FILE *f = fopen("savegame.bin", "wb");             /* b = binary */
    if (f == NULL) { perror("savegame.bin"); return 1; }
    size_t written = fwrite(slots, sizeof slots[0], 3, f);   /* raw bytes of 3 structs */
    fclose(f);
    printf("Wrote %zu save slots (%zu bytes)\n", written, written * sizeof slots[0]);

    SaveGame loaded[3];
    memset(loaded, 0, sizeof loaded);
    f = fopen("savegame.bin", "rb");
    if (f == NULL) { perror("savegame.bin"); return 1; }
    size_t got = fread(loaded, sizeof loaded[0], 3, f);
    fclose(f);
    if (got != 3) {
        printf("File was short: read %zu slots\n", got);
        return 1;
    }
    for (int i = 0; i < 3; i++)
        printf("Slot %d: %-5s level %2d, %5d gold, %.1f hours\n",
               i + 1, loaded[i].name, loaded[i].level, loaded[i].gold, loaded[i].play_hours);

    /* compare with text: the same numbers as text would be a different size */
    f = fopen("savegame.txt", "w");
    if (f == NULL) { perror("savegame.txt"); return 1; }
    for (int i = 0; i < 3; i++)
        fprintf(f, "%s %d %d %.1f\n", slots[i].name, slots[i].level, slots[i].gold, slots[i].play_hours);
    long text_size = ftell(f);
    fclose(f);
    printf("Same data as text: %ld bytes\n", text_size);

    remove("savegame.bin");
    remove("savegame.txt");
    return 0;
}
