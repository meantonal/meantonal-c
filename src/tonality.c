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
    out->chroma_offset = mode - chroma;

    return 0;
}

TonalContext context_from_chroma(int chroma, enum Mode mode) {
    TonalContext out;

    out.mode = mode;
    out.chroma_offset = mode - chroma;
    out.tonic.letter = chroma % 7 + 2;
    out.tonic.accidental = --chroma < 0 ? -chroma / 12 : chroma / 12;

    return out;
}

TonalContext context_from_pitch(Pitch p, enum Mode mode) {
    TonalContext out;

    out.tonic.letter = pitch_letter(p);
    out.tonic.accidental = pitch_accidental(p);
    out.mode = mode;
    out.chroma_offset = mode - pitch_chroma(p);

    return out;
}

enum Alteration degree_alteration(Pitch p, TonalContext key) {
    int x = pitch_chroma(p) + key.chroma_offset;
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

Pitch snap_diatonic(Pitch p, TonalContext key) {
    while (degree_alteration(p, key) > DIATONIC_DEG) {
        p.w--;
        p.h++;
    }
    while (degree_alteration(p, key) < DIATONIC_DEG) {
        p.w++;
        p.h--;
    }

    return p;
}

Pitch transpose_diatonic(Pitch p, int interval, TonalContext key) {
    return snap_diatonic(transpose_real(p, (Interval){interval, 0}), key);
}
