#ifndef INTERVAL_H
#define INTERVAL_H

#include "types.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * Parses an interval name like "P5" to generate an Interval.
 * @param out
 * Pointer to an Interval to store the resulting vector.
 * @return
 * 0 means nothing went wrong.
 */
int interval_from_name(const char *s, Interval *out);

/**
 * Create an Interval from two SPN pitch names.
 * e.g. "C4", "E4" -> major third.
 * Interval is calculated q - p, not p - q.
 * @param out
 * Pointer to an Interval to store the resulting vector.
 * @return
 * 0 means nothing went wrong.
 */
int interval_from_spn(const char *p_str, const char *q_str, Interval *out);

/**
 * Create an interval from two Pitch vectors.
 *
 * @return
 * Interval (q - p)
 */
static inline Interval interval_between(Pitch p, Pitch q) {
    return (Interval){.w = q.w - p.w, .h = q.h - p.h};
}

/**
 * The number of perfect fifths separating an interval from the unison.
 * Abstracts octave information away.
 */
static inline int interval_chroma(Interval m) { return (2 * m.w) - (5 * m.h); }

/**
 * Returns true if the Interval can occur diatonically (is found within the
 * major scale)
 */
static inline bool interval_diatonic(Interval m) {
    return abs(interval_chroma(m)) < 7;
}

/**
 * Check whether two intervals are the same.
 * Enharmonic intervals are not considered the same, use interval_enharmonic().
 */
static inline bool intervals_equal(Interval m, Interval n) {
    return (m.w == n.w && m.h == n.h);
}

/**
 * Check whether two intervals are enharmonic in a given EDO mapping.
 * If you're unsure what the last parameter means, use 12.
 * @param edo
 * The EDO tuning system to compare enharmonicity in.
 */
static inline bool intervals_enharmonic(Interval m, Interval n, int edo) {
    return (interval_chroma(m) % edo + edo) % edo ==
           (interval_chroma(n) % edo + edo) % edo;
}

/**
 * Get the number of diatonic steps subtended by an Interval.
 */
static inline int stepspan(Interval m) { return m.w + m.h; }

/**
 * Get the 12-tone pitch class interval number of an Interval.
 */
static inline int interval_pc12(Interval m) { return (2 * m.w + m.h) % 12; }

/**
 * 0 is perfect.
 * 1/-1 are major/minor.
 * 2/-2 are augmente/diminished.
 * etc. for silly intervals.
 */
static inline int interval_quality(Interval m) {
    int sign = stepspan(m) < 0 ? -1 : 1;
    int chroma = interval_chroma(m);
    if (chroma == 0)
        return sign * 0;
    if (chroma > 0 && chroma <= 5)
        return sign * (chroma + 5) / 7;
    if (chroma < 0 && chroma >= -5)
        return sign * (chroma - 5) / 7;
    if (chroma > 5)
        return sign * (chroma + 8) / 7;
    return sign * (chroma - 8) / 7;
}

/**
 * Returns the standard name of an interval as a string.
 * You must pass a char buf[8] to store the result, which is returned via an
 * out-param.
 */
void interval_name(Interval m, char *out);

/**
 * Returns the passed in interval with its values negated.
 * An ascending major 3rd becomes a descending major 3rd.
 */
static inline Interval interval_negate(Interval m) {
    return (Interval){.w = -m.w, .h = -m.h};
}

/**
 * Returns the sum of two intervals.
 */
static inline Interval intervals_add(Interval m, Interval n) {
    return (Interval){.w = m.w + n.w, .h = m.h + n.h};
}

/**
 * Returns the difference of two intervals.
 */
static inline Interval intervals_subtract(Interval m, Interval n) {
    return (Interval){.w = m.w - n.w, .h = m.h - n.h};
}

/**
 * Multiplies an Interval vector by a scalar integer.
 */
static inline Interval interval_times(Interval m, int x) {
    return (Interval){.w = m.w * x, .h = m.h * x};
}

/**
 * Reduces an interval until it is smaller than an octave
 */
static inline Interval interval_simple(Interval m) {
    while (m.w + m.h >= 7) {
        m.w -= 5;
        m.h -= 2;
    }
    while (m.w + m.h <= -7) {
        m.w += 5;
        m.h += 2;
    }
    return m;
}

#endif
