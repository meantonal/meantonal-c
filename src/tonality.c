#include "../include/tonality.h"

int context_from_str(char *s, enum Mode mode, TonalContext *out) {
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

    out->tonic.letter = letter;
    out->tonic.accidental = acc;
    out->mode = mode;
    out->offset.chroma = mode - chroma;
    out->offset.letter = letter - 2;

    return 0;
}

enum Alteration degree_alteration(Pitch p, TonalContext key) {
    int x = pitch_chroma(p) + key.offset.chroma;
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
