#include "../include/pitch.h"
#include <stdlib.h>

static const Pitch letters[7] = {
    {4, 1}, {5, 1}, {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1},
};

Pitch pitch_from_standard(StandardPitch p) {
    return (Pitch){
        .w = letters[p.letter].w + 5 * p.octave + p.accidental,
        .h = letters[p.letter].h + 2 * p.octave - p.accidental,
    };
}

int pitch_from_spn(const char *s, Pitch *out) {
    const char *p = s;

    // 1. letter name
    int letter;
    if (*p >= 'A' && *p <= 'G') {
        letter = *p++ - 'A';
    } else if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w = letters[letter].w;
    out->h = letters[letter].h;

    // 2. accidental
    int acc = 0;
    while (*p == '#' || *p == 'b' || *p == 'x' || *p == 'w') {
        switch (*p) {
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
        p++;
    }
    out->w += acc;
    out->h -= acc;

    // 3. octave (can be negative, multi-digit)
    char *end;
    long oct = strtol(p, &end, 10) + 1;
    if (end == p) {
        return 1; // no digits found
    }
    out->w += (int)oct * 5;
    out->h += (int)oct * 2;

    return 0;
}
