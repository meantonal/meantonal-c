#include "types.h"
#include <stdbool.h>

/**
 * Converts from (letter, accidental, octave) format to (whole, half)
 */
Pitch pitch_from_standard(StandardPitch p);

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

typedef struct LilyParseContext {
    Pitch previous;
} LilyParseContext;

LilyParseContext lily_parse_context_init(Pitch p);

int pitch_from_relative_lily(
    LilyParseContext *ctx,
    const char *s,
    Pitch *out
);

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
 * @return
 * true if the Pitch is more than a quadruple sharp/flat away from a natural,
 * which usually indicates a logic error upstream, or if the name would not
 * fit in 8 characters at all, in which case "ERR" is written to out instead.
 * false otherwise.
 */
bool pitch_spn(Pitch p, char *out);

/**
 * Returns the LilyPond note name of a Pitch as a string.
 * You must pass a char buf[16] to store the result, which is returned via an
 * out-param.
 * @return
 * true if the Pitch's accidental goes beyond a double sharp/flat (LilyPond
 * has no symbol for anything more remote), or if the name would not fit in
 * 16 characters at all. In either case "ERR" is written to out instead.
 * False otherwise.
 */
bool pitch_lily(Pitch p, char *out);

/**
 * Returns the Helmholtz note name of a Pitch as a string.
 * You must pass a char buf[16] to store the result, which is returned via an
 * out-param.
 * @return
 * true if the Pitch is more than a quadruple sharp/flat away from a natural,
 * which usually indicates a logic error upstream, or if the name would not
 * fit in 16 characters at all, in which case "ERR" is written to out instead.
 * false otherwise.
 */
bool pitch_helmholtz(Pitch p, char *out);

/**
 * Returns the ABC note name of a Pitch as a string.
 * You must pass a char buf[16] to store the result, which is returned via an
 * out-param.
 * @return
 * true if the Pitch's accidental goes beyond a double sharp/flat (ABC
 * notation has no symbol for anything more remote), or if the name would not
 * fit in 16 characters at all. In either case "ERR" is written to out instead.
 * False otherwise.
 */
bool pitch_abc(Pitch p, char *out);

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
