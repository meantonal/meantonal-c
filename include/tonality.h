#ifndef TONALITY_H
#define TONALITY_H

#include "pitch.h"
#include "types.h"

/**
 * Creates a TonalContext from a string naming the pitch class (e.g. "Ab"), and
 * a Mode (e.g. DORIAN).
 * @param out
 * Pointer to a TonalContext to store the resulting data.
 * @return
 * 0 means nothing went wrong.
 */
int context_from_str(char *s, enum Mode mode, TonalContext *out);

/**
 * Creates a TonalContext from a specified chroma (signed distance in 5ths from
 * C) and Mode.
 */
TonalContext context_from_chroma(int chroma, enum Mode mode);

/**
 * Creates a TonalContext from a passed in Pitch vector and specified Mode.
 */
TonalContext context_from_pitch(Pitch p, enum Mode mode);

/**
 * Returns the degree number represented by the given Pitch in the given
 * TonalContext, 0-indexed so the tonic is 0.
 */
static inline enum Degree degree_number(Pitch p, TonalContext key) {
    return (enum Degree)(((p.w + p.h - key.tonic.letter + 2) % 7 + 7) % 7);
}

/**
 * 0 is diatonic
 * 1/-1 is raised/lowered
 * 2/-2 is too sharp/too flat to function in the specified key.
 */
enum Alteration degree_alteration(Pitch p, TonalContext key);

/**
 * Returns the chroma of the diatonic version of the given degree in the given
 * TonalContext.
 */
static inline int degree_chroma(enum Degree degree, TonalContext key) {
    return (degree * 2 + key.mode) % 7 - key.chroma_offset;
}

/**
 * Snaps a given pitch to the version of that letter diatonic in the given
 * TonalContext.
 */
Pitch snap_diatonic(Pitch p, TonalContext key);

/**
 * Transpose a Pitch by a generic interval to a note diatonic in the given
 * TonalContext.
 */
Pitch transpose_diatonic(Pitch p, int interval, TonalContext key);

#endif
