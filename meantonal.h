#include <stdbool.h>
#include <stdlib.h>

// -----------------------------------------
// HEADER DECLARATIONS ---------------------
// -----------------------------------------

#ifndef MEANTONAL_HEADER
#define MEANTONAL_HEADER

/**
 * The most fundamental pitch representation in Meantonal.
 */
typedef struct {
    int w; // whole steps
    int h; // half steps
} Pitch;

/**
 * Intervals represent difference vectors between two Pitch vectors.
 */
typedef struct {
    int w; // whole steps
    int h; // half steps
} Interval;

enum Mode {
    LYDIAN,
    IONIAN,
    MIXOLYDIAN,
    DORIAN,
    AEOLIAN,
    PHRYGIAN,
    LOCRIAN,

    MAJOR = IONIAN,
    MINOR = AEOLIAN
};

/**
 * This type is used with functions that reconcile Pitch vectors into a position
 * in a key/mode.
 */
typedef struct {
    int chroma;
    enum Mode mode;
} Key;

/**
 * The Map1d represents a 1x2 matrix for mapping Pitch vectors down to one
 * dimension, e.g. (2, 1) maps Pitch vectors to MIDI.
 */
typedef struct {
    int m0, m1;
} Map1d;

/**
 * The Map2d represents a 2x2 matrix to map from one 2d vector representation to
 * another. Useful for changing coordinate basis for rendering alternative
 * isomorphic keyboard layouts.
 */
typedef struct {
    int m00, m01;
    int m10, m11;
} Map2d;

/**
 * This type is to distinguish vectors after applying 2d maps (change of
 * basis), so they are not accidentally used as regular Pitch or Interval
 * vectors.
 */
typedef struct {
    int x;
    int y;
} MappedVec;

/**
 * This type is used with functions that invert Pitches about a fixed point.
 */
typedef struct {
    int w;
    int h;
} MirrorAxis;

/**
 * An alternative pitch representation for:
 * - Pretty printing
 * - Rendering standard notation
 * - Potentially an easier parsing target
 */
typedef struct {
    int letter;
    int accidental;
    int octave;
} StandardPitch;

extern const Map1d ET7, ET12, ET19, ET31, ET50, ET55;
extern const Map2d WICKI_TO, WICKI_FROM, GENERATORS_TO, GENERATORS_FROM;

/**
 * Parses Scientific Pitch Notation to generate a pitch.
 * @param out
 * Pointer to a Pitch to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int pitch_from_spn(const char *s, Pitch *out);

/**
 * @brief
 * The number of perfect fifths separating a Pitch from C.
 * Abstracts octave information away.
 */
static inline int pitch_chroma(Pitch p) { return 2 * p.w - 5 * p.h; }

/**
 * @brief
 * Returns the letter number of a Pitch.
 * To convert to an actual letter, just add 'a' or 'A'.
 */
static inline int pitch_letter(Pitch p) { return (p.w + p.h + 2) % 7; }

/**
 * @brief
 * 0 is natural.
 * Sharps are positive.
 * Flats are negative.
 */
static inline int pitch_accidental(Pitch p) {
    int chroma = pitch_chroma(p) + 1;
    return chroma < 0 ? chroma / 7 - 1 : chroma / 7;
}

/**
 * @brief
 * Returns the SPN octave number of a Pitch (C4 is middle C)
 */
static inline int pitch_octave(Pitch p) {
    return p.w + p.h < 0 ? (p.w + p.h) / 7 - 2 : (p.w + p.h) / 7 - 1;
}

/**
 * @brief
 * Returns the standard MIDI value for a given Pitch.
 */
static inline int pitch_midi(Pitch p) { return 2 * p.w + p.h; }

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
 * @brief
 * Returns a new Pitch shifted by the given interval.
 * @return
 * Pitch (p + m)
 */
static inline Pitch transpose_real(Pitch p, Interval m) {
    return (Pitch){.w = p.w + m.w, .h = p.h + m.h};
}

/**
 * Maps a pitch to an integer using a 1x2 matrix.
 * Most built-in functions that take Pitches and return integers perform
 * this operation somewhere along the way.
 */
static inline int pitch_map_1d(Pitch p, Map1d T) {
    return T.m0 * p.w + T.m1 * p.h;
}

/**
 * Maps a Pitch vector to a MappedVec type using a 2x2 matrix.
 * MappedVec is a special type to ensure it is not accidentally operated with
 * as if it is a regular Pitch.
 */
static inline MappedVec pitch_map_2d(Pitch p, Map2d T) {
    return (MappedVec){.x = T.m00 * p.w + T.m01 * p.h,
                       .y = T.m10 * p.w + T.m11 * p.h};
}

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

static inline Pitch pitch_invert(Pitch p, MirrorAxis a) {
    return (Pitch){.w = a.w - p.w, .h = a.h - p.h};
}

/**
 * @brief
 * Converts from (whole, half) format to (letter, accidental, octave)
 */
static inline StandardPitch pitch_to_standard(Pitch p) {
    return (StandardPitch){.letter = pitch_letter(p),
                           .accidental = pitch_accidental(p),
                           .octave = pitch_octave(p)};
}

/**
 * @brief
 * Converts from (letter, accidental, octave) format to (whole, half)
 */
Pitch pitch_from_standard(StandardPitch p);
static inline MirrorAxis axis_create(Pitch p, Pitch q) {
    return (MirrorAxis){.w = p.w + q.w, .h = p.h + q.h};
}

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
 * @brief
 * Create an interval from two Pitch vectors.
 *
 * @return
 * Interval (q - p)
 */
static inline Interval interval_between(Pitch p, Pitch q) {
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

/**
 * @brief
 * Returns the sum of two intervals
 * To take the difference, use interval_between((Pitch)m, n)
 * rather than intervals_add(interval_negate(m), n), as it's faster.
 */
static inline Interval intervals_add(Interval m, Interval n) {
    return (Interval){.w = m.w + n.w, .h = m.h + n.h};
}

/**
 * @brief
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

int key_from_str(char *s, enum Mode mode, Key *out);
#endif // MEANTONAL_HEADER

// -----------------------------------------
// IMPLEMENTATION --------------------------
// -----------------------------------------

#ifdef MEANTONAL
#undef MEANTONAL

const Map1d ET7 = {1, 1};
const Map1d ET12 = {2, 1};
const Map1d ET19 = {3, 2};
const Map1d ET31 = {5, 3};
const Map1d ET50 = {5, 3};
const Map1d ET55 = {5, 4};

const Map2d WICKI_TO = {1, -3, 0, 1};
const Map2d WICKI_FROM = {1, 3, 0, 1};

const Map2d GENERATORS_TO = {2, -5, -1, 3};
const Map2d GENERATORS_FROM = {3, 5, 1, 2};

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

static const Interval major_ints[7] = {
    {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
};

int interval_from_name(const char *s, Interval *out) {
    // 1. sign
    bool negative = false;
    if (*s == '-') {
        negative = true;
        s++;
    }

    // 2. quality (unadjusted)
    int quality = 0;
    while (*s == 'P' || *s == 'p' || *s == 'M' || *s == 'm' || *s == 'b' ||
           *s == '#' || *s == 'A' || *s == 'a' || *s == 'D' || *s == 'd') {
        switch (*s) {
        case 'A':
        case 'a':
        case '#':
            quality++;
            break;
        case 'm':
        case 'b':
            quality--;
            break;
        case 'D':
        case 'd':
            if (quality == 0)
                quality -= 2;
            else
                quality--;
            break;
        }
        s++;
    }

    // 3. generic size / octave
    char *end;
    long generic = strtol(s, &end, 10) - 1;
    if (end == s) {
        return 1; // no digits found
    }
    int simple = generic % 7;
    int octave = generic / 7;

    // 4. construct output pitch
    out->w = major_ints[simple].w;
    out->h = major_ints[simple].h;

    // 5. adjust octave
    out->w += 5 * octave;
    out->h += 2 * octave;

    // 6. adjust quality
    if (quality < 0 && (simple == 0 || simple == 3 || simple == 4))
        quality++;
    out->w += quality;
    out->h -= quality;

    // 7. account for sign
    if (negative) {
        out->w = -out->w;
        out->h = -out->h;
    }

    return 0;
}

int interval_from_spn(const char *p_str, const char *q_str, Interval *out) {
    Pitch p, q;
    if (pitch_from_spn(p_str, &p))
        return 1;
    if (pitch_from_spn(q_str, &q))
        return 1;
    out->w = q.w - p.w;
    out->h = q.h - p.h;
    return 0;
}

int key_from_str(char *s, enum Mode mode, Key *out) {
    // 1. letter name
    int letter, chroma;
    if (*s >= 'A' && *s <= 'G') {
        letter = *s++ - 'A';
    } else if (*s >= 'a' && *s <= 'g') {
        letter = *s++ - 'a';
    } else {
        return 1; // invalid
    }
    chroma = (letter * 2 + 4) % 7;

    // 2. accidental
    int acc = 0;
    while (*s == '#' || *s == 'b' || *s == 'x' || *s == 'w') {
        switch (*s) {
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
        s++;
    }
    chroma += 7 * acc;

    out->chroma = chroma;
    out->mode = mode;

    return 0;
}
#endif // MEANTONAL
