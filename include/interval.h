#ifndef INTERVAL_H
#define INTERVAL_H

#include "types.h"
#include <stdbool.h>

/**
 * Parses an interval name like "P5" to generate an Interval.
 * @param out
 * Pointer to an Interval to store the resulting vector.
 * @return
 * 0 means nothing went wrong.
 */
int interval_from_name(const char *s, Interval *out);

/**
 * Create an Interval from two SPN note names.
 * e.g. "C4", "E4" -> major third.
 * Interval is calculated q - p, not p - q.
 * @param out
 * Pointer to an Interval to store the resulting vector.
 * @return
 * 0 means nothing went wrong.
 */
int interval_from_spn(const char *p_str, const char *q_str, Interval *out);

/**
 * @brief
 * Create an interval from two Note vectors.
 *
 * @return
 * Interval (q - p)
 */
static inline Interval interval_between(Note p, Note q) {
    return (Interval){.w = q.w - p.w, .h = q.h - p.h};
}

/**
 * @brief
 * The number of perfect fifths separating an interval from the unison.
 * Abstracts octave information away.
 */
static inline int interval_chroma(Interval m) { return (2 * m.w) - (5 * m.h); }

/**
 * @brief
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
 * @brief
 * Get the number of diatonic steps subtended by an Interval.
 */
static inline int stepspan(Interval m) { return m.w + m.h; }

/**
 * @brief
 * 0 is perfect.
 * 1/-1 are major/minor.
 * 2/-2 are augmente/diminished.
 * etc. for silly intervals.
 */
static inline int interval_quality(Interval m) {
    int chroma = interval_chroma(m);
    if (chroma == 0)
        return 0;
    if (chroma > 0 && chroma <= 5)
        return (chroma + 5) / 7;
    if (chroma < 0 && chroma >= -5)
        return (chroma - 5) / 7;
    if (chroma > 5)
        return (chroma + 8) / 7;
    return (chroma - 8) / 7;
}

/**
 * @brief
 * Returns the passed in interval with its values negated.
 * An ascending major 3rd becomes a descending major 3rd.
 */
static inline Interval interval_negate(Interval m) {
    return (Interval){.w = -m.w, .h = -m.h};
}

#endif
