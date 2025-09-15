#ifndef NOTE_H
#define NOTE_H

#include "types.h"
#include <stdbool.h>

/**
 * Parses Scientific Pitch Notation to generate a pitch.
 * @param out
 * Pointer to a Pitch to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int pitch_from_spn(const char *s, Pitch *out);

/**
 * Parses a LilyPond pitch name to generate a pitch.
 * @param out
 * Pointer to a Pitch to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int pitch_from_lily(const char *s, Pitch *out);

/**
 * Parses a Helmholtz pitch name to generate a pitch.
 * @param out
 * Pointer to a Pitch to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int pitch_from_helmholtz(const char *s, Pitch *out);

/**
 * Parses an ABC note name to generate a pitch.
 * @param out
 * Pointer to a Pitch to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int pitch_from_abc(const char *s, Pitch *out);

/**
 * Creates a Pitch vector from a specified chroma (signed distance from C in
 * 5ths) and octave (following SPN octave numbering).
 */
Pitch pitch_from_chroma(int chroma, int octave);

/**
 * The number of perfect fifths separating a Pitch from C.
 * Abstracts octave information away.
 */
static inline int pitch_chroma(Pitch p) { return 2 * p.w - 5 * p.h; }

/**
 * Returns the letter number of a Pitch.
 * To convert to an actual letter, just add 'a' or 'A'.
 */
static inline int pitch_letter(Pitch p) {
    return ((p.w + p.h + 2) % 7 + 7) % 7;
}

/**
 * 0 is natural.
 * Sharps are positive.
 * Flats are negative.
 */
static inline int pitch_accidental(Pitch p) {
    int chroma = pitch_chroma(p) + 1;
    return chroma < 0 ? chroma / 7 - 1 : chroma / 7;
}

/**
 * Returns the SPN octave number of a Pitch (C4 is middle C)
 */
static inline int pitch_octave(Pitch p) {
    return p.w + p.h < 0 ? (p.w + p.h) / 7 - 2 : (p.w + p.h) / 7 - 1;
}

/**
 * Returns the SPN name of a Pitch as a string.
 * You must pass a char buf[8] to store the result, which is returned via an
 * out-param.
 */
void pitch_spn(Pitch p, char *out);

/**
 * Returns the standard MIDI value for a given Pitch.
 */
static inline int pitch_midi(Pitch p) { return 2 * p.w + p.h; }

/**
 * Returns the 12-tone pitch class of a given Pitch.
 * C is 0.
 */
static inline int pitch_pc12(Pitch p) { return pitch_midi(p) % 12; }

/**
 * Returns the (signed) distance in diatonic steps between two Pitch vectors.
 */
static inline int steps_between(Pitch p, Pitch q) {
    return (q.w + q.h) - (p.w + p.h);
}

/**
 * Check whether two pitches are the same.
 * Enharmonic pitches are not considered the same, use pitch_enharmonic().
 */
static inline bool pitches_equal(Pitch p, Pitch q) {
    return (p.w == q.w && p.h == q.h);
}

/**
 * Check whether two Pitches are enharmonic in a given EDO mapping.
 * If you're unsure what the last parameter means, use 12.
 * @param edo
 * The EDO tuning system to compare enharmonicity in.
 */
static inline bool pitches_enharmonic(Pitch m, Pitch n, int edo) {
    return (pitch_chroma(m) % edo + edo) % edo ==
           (pitch_chroma(n) % edo + edo) % edo;
}

/**
 * Returns a new Pitch shifted by the given interval.
 * @return
 * Pitch (p + m)
 */
static inline Pitch transpose_real(Pitch p, Interval m) {
    return (Pitch){.w = p.w + m.w, .h = p.h + m.h};
}

/**
 * Creates a MirrorAxis about which to invert Pitch vectors from two input
 * Pitches that will map to each other.
 */
static inline MirrorAxis axis_create(Pitch p, Pitch q) {
    return (MirrorAxis){.w = p.w + q.w, .h = p.h + q.h};
}

/**
 * Creates a MirrorAxis about which to invert Pitch vectors from two input SPN
 * strings representing Pitches that will map to each other.
 * @param out
 * Pointer to a MirrorAxis to store the resulting axis.
 * @return
 * 0 means nothing went wrong.
 */
static inline int axis_from_spn(char *p_str, char *q_str, MirrorAxis *out) {
    Pitch p, q;

    if (pitch_from_spn(p_str, &p))
        return 1;
    if (pitch_from_spn(q_str, &q))
        return 1;

    out->w = p.w + q.w;
    out->h = p.h + q.h;
    return 0;
}

/**
 * Inverts a Pitch about a given MirrorAxis
 */
static inline Pitch pitch_invert(Pitch p, MirrorAxis a) {
    return (Pitch){.w = a.w - p.w, .h = a.h - p.h};
}

/**
 * Converts from (whole, half) format to (letter, accidental, octave)
 */
static inline StandardPitch pitch_to_standard(Pitch p) {
    return (StandardPitch){.letter = pitch_letter(p),
                           .accidental = pitch_accidental(p),
                           .octave = pitch_octave(p)};
}

/**
 * Converts from (letter, accidental, octave) format to (whole, half)
 */
Pitch pitch_from_standard(StandardPitch p);

#endif
