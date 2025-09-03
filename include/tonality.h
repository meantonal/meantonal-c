#ifndef TONALITY_H
#define TONALITY_H

#include "pitch.h"
#include "types.h"

int context_from_str(char *s, enum Mode mode, TonalContext *out);

static inline int degree_number(Pitch p, TonalContext key) {
    return ((p.w + p.h - key.offset.letter) % 7 + 7) % 7;
}

enum Alteration degree_alteration(Pitch p, TonalContext key);

#endif
