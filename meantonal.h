#include <stdlib.h>
#include <stdbool.h>

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
} Note;

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
} StandardNote;

/**
 * Intervals represent difference vectors between two Note vectors.
 */
typedef struct {
    int w; // whole steps
    int h; // half steps
} Interval;

/**
 * The Map1d represents a 1x2 matrix for mapping Note vectors down to one
 * dimension, e.g. (2, 1) maps Note vectors to MIDI.
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
 * basis), so they are not accidentally used as regular Note or Interval
 * vectors.
 */
typedef struct {
    int x;
    int y;
} MappedVec;

/**
 * This type is used with functions that invert Notes about a fixed point.
 */
typedef struct {
    int w;
    int h;
} MirrorAxis;



extern const Map1d ET7, ET12, ET19, ET31, ET50, ET55;
extern const Map2d WICKI_TO, WICKI_FROM, GENERATORS_TO, GENERATORS_FROM;



/**
 * @brief
 * The number of perfect fifths separating a Note from C.
 * Abstracts octave information away.
 */
static inline int note_chroma(Note p) { return 2 * p.w - 5 * p.h; }

/**
 * Check whether two notes are the same.
 * Enharmonic notes are not considered the same, use note_enharmonic().
 */
static inline bool notes_equal(Note p, Note q) {
    return (p.w == q.w && p.h == q.h);
}

/**
 * Check whether two Notes are enharmonic in a given EDO mapping.
 * If you're unsure what the last parameter means, use 12.
 * @param edo
 * The EDO tuning system to compare enharmonicity in.
 */
static inline bool notes_enharmonic(Note m, Note n, int edo) {
    return (note_chroma(m) % edo + edo) % edo ==
           (note_chroma(n) % edo + edo) % edo;
}

/**
 * @brief
 * Returns the standard MIDI value for a given Note.
 */
static inline int note_midi(Note p) { return 2 * p.w + p.h; }

/**
 * @brief
 * Returns the letter number of a Note.
 * To convert to an actual letter, just add 'a' or 'A'.
 */
static inline int note_letter(Note p) { return (p.w + p.h + 2) % 7; }

/**
 * @brief
 * 0 is natural.
 * Sharps are positive.
 * Flats are negative.
 */
static inline int note_accidental(Note p) {
    int chroma = note_chroma(p) + 1;
    return chroma < 0 ? chroma / 7 - 1 : chroma / 7;
}

/**
 * @brief
 * Returns the SPN octave number of a Note (C4 is middle C)
 */
static inline int note_octave(Note p) {
    return p.w + p.h < 0 ? (p.w + p.h) / 7 - 2 : (p.w + p.h) / 7 - 1;
}

/**
 * @brief
 * Converts from (whole, half) format to (letter, accidental, octave)
 */
static inline StandardNote note_to_standard(Note p) {
    return (StandardNote){.letter = note_letter(p),
                          .accidental = note_accidental(p),
                          .octave = note_octave(p)};
}

/**
 * @brief
 * Converts from (letter, accidental, octave) format to (whole, half)
 */
Note standard_to_note(StandardNote p);

/**
 * Maps a Note vector to a MappedVec type using a 2x2 matrix.
 * MappedVec is a special type to ensure it is not accidentally operated with
 * as if it is a regular Note.
 */
static inline MappedVec note_map_2d(Note p, Map2d T) {
    return (MappedVec){.x = T.m00 * p.w + T.m01 * p.h,
                       .y = T.m10 * p.w + T.m11 * p.h};
}

/**
 * Maps a note to an integer using a 1x2 matrix.
 * Most built-in functions that take Notes and return integers perform
 * this operation somewhere along the way.
 */
static inline int note_map_1d(Note p, Map1d T) {
    return T.m0 * p.w + T.m1 * p.h;
}

/**
 * Parses Scientific Pitch Notation to generate a note.
 * @param out
 * Pointer to a Note to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int note_from_spn(const char *s, Note *out);

static inline MirrorAxis axis_create(Note p, Note q) {
    return (MirrorAxis){.w = p.w + q.w, .h = p.h + q.h};
}

static inline int axis_from_spn(char *p_str, char *q_str, MirrorAxis *out) {
    Note p, q;

    if (note_from_spn(p_str, &p))
        return 1;
    if (note_from_spn(q_str, &q))
        return 1;

    out->w = p.w + q.w;
    out->h = p.h + q.h;
    return 0;
}

static inline Note note_invert(Note p, MirrorAxis a) {
    return (Note){.w = a.w - p.w, .h = a.h - p.h};
}



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
 * Returns a new Note shifted by the given interval.
 * @return
 * Note (p + m)
 */
static inline Note transpose_real(Note p, Interval m) {
    return (Note){.w = p.w + m.w, .h = p.h + m.h};
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
 * Create an Interval from two SPN note names.
 * e.g. "C4", "E4" -> major third.
 * Interval is calculated q - p, not p - q.
 * @param out
 * Pointer to an Interval to store the resulting vector.
 * @return
 * 0 means nothing went wrong.
 */
int interval_from_spn(const char *p_str, const char *q_str, Interval *out);

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

static const Note letters[7] = {
    {4, 1}, {5, 1}, {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1},
};

Note standard_to_note(StandardNote p) {
    return (Note){
        .w = letters[p.letter].w + 5 * p.octave + p.accidental,
        .h = letters[p.letter].h + 2 * p.octave - p.accidental,
    };
}

int note_from_spn(const char *s, Note *out) {
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

    // 4. construct output note
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
    Note p, q;
    if (note_from_spn(p_str, &p))
        return 1;
    if (note_from_spn(q_str, &q))
        return 1;
    out->w = q.w - p.w;
    out->h = q.h - p.h;
    return 0;
}
#endif // MEANTONAL

