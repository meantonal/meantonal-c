#ifndef TONALITY_H
#define TONALITY_H

#include "pitch.h"
#include "types.h"

int context_from_str(char *s, enum Mode mode, TonalContext *out);

TonalContext context_from_chroma(int chroma, enum Mode mode);

TonalContext context_from_pitch(Pitch p, enum Mode mode);

static inline enum Degree degree_number(Pitch p, TonalContext key) {
    return (enum Degree)(((p.w + p.h - key.tonic.letter + 2) % 7 + 7) % 7);
}

enum Alteration degree_alteration(Pitch p, TonalContext key);

static inline int degree_chroma(enum Degree degree, TonalContext key) {
    return (degree * 2 + key.mode) % 7 - key.chroma_offset;
}

Pitch snap_diatonic(Pitch p, TonalContext key);

Pitch transpose_diatonic(Pitch p, int interval, TonalContext key);

#endif
