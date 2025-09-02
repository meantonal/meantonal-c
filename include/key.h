#ifndef KEY_H
#define KEY_H

#include "pitch.h"
#include "types.h"

int key_from_str(char *s, enum Mode mode, Key *out);

static inline int degree_number(Pitch p, Key k) {
    return ((pitch_chroma(p) - k.chroma) * 4 % 7 + 7) % 7;
}

enum Alteration degree_alteration(Pitch p, Key k);

#endif
