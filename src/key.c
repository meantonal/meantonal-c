#include "../include/key.h"

int key_from_str(char *s, enum Mode mode, Key *out) {
    // 1. letter name
    int letter, chroma;
    if (*s >= 'A' && *s <= 'G') {
        letter = *s++ - 'A';
    } else if (*s >= 'a' && *s <= 'g') {
        letter = *s++ - 'a';
    } else {
        return 1; // invalid
    }
    chroma = (letter * 2 + 4) % 7 - 1; // trust me it works.

    // 2. accidental
    int acc = 0;
    while (*s == '#' || *s == 'b' || *s == 'x' || *s == 'w') {
        switch (*s) {
        case '#':
            acc++;
            break;
        case 'b':
            acc--;
            break;
        case 'x':
            acc += 2;
            break;
        case 'w':
            acc -= 2;
            break;
        }
        s++;
    }
    chroma += 7 * acc;

    out->chroma = chroma;
    out->mode = mode;

    return 0;
}

enum Alteration degree_alteration(Pitch p, Key k) {
    int x = pitch_chroma(p) + k.mode - k.chroma;
    if (0 <= x && x < 7)
        return DIATONIC_DEG;
    if (7 <= x && x < 12)
        return RAISED_DEG;
    if (-5 <= x && x < 0)
        return LOWERED_DEG;
    if (x < -5)
        return FOREIGN_DEG_FLAT;
    return FOREIGN_DEG_SHARP;
}
