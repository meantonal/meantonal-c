#include "types.h"

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
 * Returns the SPN name of a Pitch as a string.
 * You must pass a char buf[8] to store the result, which is returned via an
 * out-param.
 */
void pitch_spn(Pitch p, char *out);

/**
 * Returns the LilyPond note name of a Pitch as a string.
 * You must pass a char buf[16] to store the result, which is returned via an
 * out-param.
 * Will truncate anything past C-1 and B9, and accidentals more remote than
 * quadruple sharps/flats.
 */
void pitch_lily(Pitch p, char *out);

/**
 * Returns the Helmholtz note name of a Pitch as a string.
 * You must pass a char buf[16] to store the result, which is returned via an
 * out-param.
 * Will truncate anything past C-1 and B9, and accidentals more remote than
 * quadruple sharps/flats.
 */
void pitch_helmholtz(Pitch p, char *out);

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
