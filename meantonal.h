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
} Pitch;

/**
 * Intervals represent difference vectors between two Pitch vectors.
 */
typedef Pitch Interval;

/**
 * Enum of modes numbered in descending fifths starting from Lydian = 0.
 */
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
        struct {
                int letter;
                int accidental;
        } tonic;
        enum Mode mode;
        int chroma_offset; // offset used by internal functions to reconcile
                           // notes.
} TonalContext;

/**
 * Enum for indicating scale degrees (0-indexed) by their functional names.
 */
enum Degree {
    TONIC,
    SUPERTONIC,
    MEDIANT,
    SUBDOMINANT,
    DOMINANT,
    SUBMEDIANT,
    SUBTONIC
};

/**
 * Enum for indicating the alteration of scale degrees within a key or mode.
 */
enum Alteration {
    FOREIGN_DEG_FLAT = -2,
    LOWERED_DEG,
    DIATONIC_DEG,
    RAISED_DEG,
    FOREIGN_DEG_SHARP
};

/**
 * Type for constructing arbitrary pitch class sets.
 * Initialise it to NULL and use pc_set_insert to add pitch classes by chroma.
 * You are responsible for calling pc_set_destroy to free up resources.
 */
typedef struct tnode *PitchClassSet;

/**
 * The Map1D represents a 1x2 matrix for mapping Pitch vectors down to one
 * dimension, e.g. (2, 1) maps Pitch vectors to MIDI.
 */
typedef struct {
        double m0, m1;
} Map1D;

/**
 * The Map2D represents a 2x2 matrix to map from one 2D vector representation to
 * another. Useful for changing coordinate basis for rendering alternative
 * isomorphic keyboard layouts.
 */
typedef struct {
        double m00, m01;
        double m10, m11;
} Map2D;

/**
 * This type is reserved for operations using Map2D matrices.
 */
typedef struct {
        double x, y;
} MapVec;

/**
 * The TuningMap type is used to render frequencies, cent values and ratios
 * from Pitch and Interval vectors.
 */
typedef struct {
        Pitch ref_pitch;
        double ref_freq;
        Map1D centmap;
} TuningMap;

/**
 * This type is used with functions that invert Pitches about a fixed point.
 */
typedef Pitch MirrorAxis;

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



extern const Map1D EDO7, EDO12, EDO17, EDO19, EDO22, EDO31, EDO50, EDO53, EDO55,
    EDO81;
extern const Map2D WICKI_TO, WICKI_FROM, GENERATORS_TO, GENERATORS_FROM;

extern const double CONCERT_C4;



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



/**
 * Frees the memory previously allocated by a passed in PitchClassSet.
 */
void pc_set_destroy(PitchClassSet set);

/**
 * Adds a pitch class to a PitchClassSet by chroma.
 * @return
 * The enlarged set.
 */
void pc_set_insert(PitchClassSet *set, int chroma);

/**
 * Checks whether a given pitch class is in a PitchClassSet by its chroma.
 */
bool pc_set_contains(PitchClassSet set, int chroma);

/**
 * Creates a copy of a pitch class set transposed by the specified number of
 * fifths.
 */
PitchClassSet pc_set_transpose(PitchClassSet set, int offset);

/**
 * Creates a copy of a pitch class set inverted about a given axis. An easy way
 * to obtain the axis is by creating a MirrorAxis and then taking its chroma.
 */
PitchClassSet pc_set_invert(PitchClassSet set, int axis);

/**
 * Creates a new set that contains the elements of both passed in sets.
 */
PitchClassSet pc_set_union(PitchClassSet a, PitchClassSet b);

/**
 * Creates a new set that contains the common elements shared by the passed in
 * sets.
 */
PitchClassSet pc_set_intersection(PitchClassSet a, PitchClassSet b);

/**
 * Creates a new set that contains elements in the first set not shared by the
 * second.
 */
PitchClassSet pc_set_difference(PitchClassSet a, PitchClassSet b);



/**
 * Maps to an integer using a 1x2 matrix.
 * Most built-in functions that take Pitches and return integers perform
 * this operation somewhere along the way.
 */
static inline double map_to_1d(MapVec v, Map1D T) {
    return T.m0 * v.x + T.m1 * v.y;
}

/**
 * Composes a Map1D with a Map2D to create a single Map1D operation.
 * Useful for avoiding unneccesary repeated computation.
 */
static inline Map1D map_compose_1d_2d(Map1D A, Map2D B) {
    return (Map1D){.m0 = A.m0 * B.m00 + A.m1 * B.m10,
                   .m1 = A.m0 * B.m01 + A.m1 * B.m11};
}

/**
 * Maps to a 2D MapVec type using a 2x2 matrix.
 * You must cast the result to a Pitch or Interval if you intend to use it as
 * one.
 */
static inline MapVec map_to_2d(MapVec v, Map2D T) {
    return (MapVec){.x = T.m00 * v.x + T.m01 * v.y,
                    .y = T.m10 * v.x + T.m11 * v.y};
}

/**
 * Composes a Map2D with another Map2D to create a single Map2D composite.
 * Useful for avoiding unnecessary repeated computation.
 */
static inline Map2D map_compose_2d_2d(Map2D A, Map2D B) {
    return (Map2D){.m00 = A.m00 * B.m00 + A.m01 * B.m10,
                   .m01 = A.m00 * B.m01 + A.m01 * B.m11,
                   .m10 = A.m10 * B.m00 + A.m11 * B.m10,
                   .m11 = A.m10 * B.m01 + A.m11 * B.m11};
}

/**
 * Creates a TuningMap from the width of the perfect fifth in cents in the
 * target tuning system.
 * Also required a reference Pitch and frequency for that Pitch.
 */
TuningMap tuning_map_from_fifth(double fifth, Pitch ref_pitch, double ref_freq);

/**
 * Creates a TuningMap for an EDO tuning system from the number of parts the
 * octave is to be divided into.
 * Also required a reference Pitch and frequency for that Pitch.
 */
TuningMap tuning_map_from_edo(int edo, Pitch ref_pitch, double ref_freq);

/**
 * Returns the frequency of a Pitch when rendered in the tuning system defined
 * by the passed-in TuningMap.
 */
double to_hz(Pitch p, TuningMap T);

/**
 * Returns the ratio of an Interval when rendered in the tuning system defined
 * by the passed_in TuningMap.
 */
double to_ratio(Interval m, TuningMap T);

/**
 * Returns the size on an Interval in cents when rendered in the tuning system
 * defined by the passed-in TuningMap.
 */
double to_cents(Interval m, TuningMap T);

#endif // MEANTONAL_HEADER

// -----------------------------------------
// IMPLEMENTATION --------------------------
// -----------------------------------------

#ifdef MEANTONAL
#undef MEANTONAL

const Map1D EDO7 = {1, 1};
const Map1D EDO12 = {2, 1};
const Map1D EDO17 = {3, 1};
const Map1D EDO19 = {3, 2};
const Map1D EDO22 = {4, 1};
const Map1D EDO31 = {5, 3};
const Map1D EDO50 = {5, 3};
const Map1D EDO53 = {9, 4};
const Map1D EDO55 = {5, 4};
const Map1D EDO81 = {13, 8};

const Map2D WICKI_TO = {1, -3, 0, 1};
const Map2D WICKI_FROM = {1, 3, 0, 1};

const Map2D GENERATORS_TO = {2, -5, -1, 3};
const Map2D GENERATORS_FROM = {3, 5, 1, 2};

const double CONCERT_C4 = 261.6255653005986;

static const Pitch letters[7] = {
    {4, 1}, {5, 1}, {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1},
};

int pitch_from_spn(const char *s, Pitch *out) {
    const char *p = s;

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

    char *end;
    long oct = strtol(p, &end, 10) + 1;
    if (end == p) {
        return 1; // no digits found
    }
    out->w += (int)oct * 5;
    out->h += (int)oct * 2;

    return 0;
}

int pitch_from_lily(const char *s, Pitch *out) {
    const char *p = s;

    int letter;
    if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w = letters[letter].w;
    out->h = letters[letter].h;

    int acc = 0;
    while (*p == 'i' || *p == 'e') {
        switch (*p) {
        case 'i':
            acc++;
            break;
        case 'e':
            acc--;
            break;
        }
        p += 2;
    }
    out->w += acc;
    out->h -= acc;

    int oct = 4;
    while (*p == '\'' || *p == ',') {
        switch (*p) {
        case '\'':
            oct++;
            break;
        case ',':
            oct--;
            break;
        }
        p++;
    }
    out->w += oct * 5;
    out->h += oct * 2;

    return 0;
}

int pitch_from_helmholtz(const char *s, Pitch *out) {
    const char *p = s;

    int letter;
    int oct = 4;
    if (*p >= 'A' && *p <= 'G') {
        letter = *p++ - 'A';
        oct--;
    } else if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w = letters[letter].w;
    out->h = letters[letter].h;

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

    while (*p == '\'' || *p == ',') {
        switch (*p) {
        case '\'':
            oct++;
            break;
        case ',':
            oct--;
            break;
        }
        p++;
    }
    out->w += oct * 5;
    out->h += oct * 2;

    return 0;
}

Pitch pitch_from_chroma(int chroma, int octave) {
    Pitch p = {0, 0};
    p.w += chroma * 3;
    p.h += chroma;
    while (pitch_octave(p) > octave) {
        p.w -= 5;
        p.h -= 2;
    }
    while (pitch_octave(p) < octave) {
        p.w += 5;
        p.h += 2;
    }
    return p;
}

Pitch pitch_from_standard(StandardPitch p) {
    return (Pitch){
        .w = letters[p.letter].w + 5 * p.octave + p.accidental,
        .h = letters[p.letter].h + 2 * p.octave - p.accidental,
    };
}

void pitch_spn(Pitch p, char *out) {
    size_t pos = 0;
    size_t cap = 8;

    char letter = pitch_letter(p) + 'A';
    int accidental = pitch_accidental(p);
    int octave = pitch_octave(p);

    pos += snprintf(out + pos, cap - pos, "%c", letter);

    switch (accidental) {
    case 2:
        pos += snprintf(out + pos, cap - pos, "x");
        break;
    case 1:
        pos += snprintf(out + pos, cap - pos, "#");
        break;
    case 0:
        break;
    case -1:
        pos += snprintf(out + pos, cap - pos, "b");
        break;
    case -2:
        pos += snprintf(out + pos, cap - pos, "bb");
        break;
    default:
        if (accidental > 0) {
            pos += snprintf(out + pos, cap - pos, "%d#", accidental);
        } else {
            pos += snprintf(out + pos, cap - pos, "%db", -accidental);
        }
        break;
    }
    pos += snprintf(out + pos, cap - pos, "%d", octave);
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

void interval_name(Interval m, char *out) {
    size_t cap = 8;

    static const char qualities[5] = {'d', 'm', 'P', 'M', 'A'};
    int8_t quality = interval_quality(m);
    int8_t generic_size = stepspan(m) + 1;
    if (quality <= 2 && quality >= -2) {
        snprintf(out, cap, "%c%hhd", qualities[quality + 2], generic_size);
    } else if (quality > 0) {
        snprintf(out, cap, "%dA%hhd", quality - 1, generic_size);
    } else {
        snprintf(out, cap, "%hhdd%hhd", -quality - 1, generic_size);
    }
}

int context_from_str(char *s, enum Mode mode, TonalContext *out) {
    // 1. letter name
    int letter, chroma;
    if (*s >= 'A' && *s <= 'G') {
        letter = *s++ - 'A';
    } else if (*s >= 'a' && *s <= 'g') {
        letter = *s++ - 'a';
    } else {
        return 1; // invalid
    }
    chroma = (letter * 2 + 4) % 7 - 1; // trust me it works.

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

    out->tonic.letter = letter;
    out->tonic.accidental = acc;
    out->mode = mode;
    out->chroma_offset = mode - chroma;

    return 0;
}

TonalContext context_from_chroma(int chroma, enum Mode mode) {
    TonalContext out;

    out.mode = mode;
    out.chroma_offset = mode - chroma;
    out.tonic.letter = (chroma * 4 % 7 + 9) % 7;
    out.tonic.accidental = ++chroma < 0 ? chroma / 7 - 1 : chroma / 7;

    return out;
}

TonalContext context_from_pitch(Pitch p, enum Mode mode) {
    TonalContext out;

    out.tonic.letter = pitch_letter(p);
    out.tonic.accidental = pitch_accidental(p);
    out.mode = mode;
    out.chroma_offset = mode - pitch_chroma(p);

    return out;
}

enum Alteration degree_alteration(Pitch p, TonalContext key) {
    int x = pitch_chroma(p) + key.chroma_offset;
    if (0 <= x && x < 7)
        return DIATONIC_DEG;
    if (7 <= x && x < 12)
        return RAISED_DEG;
    if (-5 <= x && x < 0)
        return LOWERED_DEG;
    if (x < -5)
        return FOREIGN_DEG_FLAT;
    return FOREIGN_DEG_SHARP;
}

Pitch snap_diatonic(Pitch p, TonalContext key) {
    while (degree_alteration(p, key) > DIATONIC_DEG) {
        p.w--;
        p.h++;
    }
    while (degree_alteration(p, key) < DIATONIC_DEG) {
        p.w++;
        p.h--;
    }

    return p;
}

Pitch transpose_diatonic(Pitch p, int interval, TonalContext key) {
    return snap_diatonic(transpose_real(p, (Interval){interval, 0}), key);
}

typedef struct tnode {
    int value;
    struct tnode *left;
    struct tnode *right;
} *PitchClassSet;

static struct tnode *create_tnode(int chroma) {
    struct tnode *node = malloc(sizeof(struct tnode));
    node->value = chroma;
    node->left = node->right = NULL;
    return node;
}

void pc_set_destroy(PitchClassSet set) {
    if (set->left)
        pc_set_destroy(set->left);
    if (set->right)
        pc_set_destroy(set->right);
    free(set);
}

void pc_set_insert(PitchClassSet *set, int chroma) {
    if (*set == NULL)
        *set = create_tnode(chroma);
    else if ((*set)->value > chroma)
        pc_set_insert(&(*set)->left, chroma);
    else if ((*set)->value < chroma)
        pc_set_insert(&(*set)->right, chroma);
}

bool pc_set_contains(PitchClassSet set, int chroma) {
    if (set == NULL)
        return false;
    return set->value == chroma || pc_set_contains(set->left, chroma) ||
           pc_set_contains(set->right, chroma);
}

PitchClassSet pc_set_transpose(PitchClassSet set, int offset) {
    if (set == NULL)
        return NULL;
    PitchClassSet new_node = create_tnode(set->value + offset);
    new_node->left = pc_set_transpose(set->left, offset);
    new_node->right = pc_set_transpose(set->right, offset);

    return new_node;
}

PitchClassSet pc_set_invert(PitchClassSet set, int axis) {
    if (set == NULL)
        return NULL;
    PitchClassSet new_node = create_tnode(axis - set->value);
    new_node->left = pc_set_invert(set->left, axis);
    new_node->right = pc_set_invert(set->right, axis);

    return new_node;
}

PitchClassSet pc_set_clone(PitchClassSet set) {
    if (set == NULL)
        return NULL;
    PitchClassSet copy = create_tnode(set->value);
    copy->left = pc_set_clone(set->left);
    copy->right = pc_set_clone(set->right);
    return copy;
}

void pc_set_insert_all(PitchClassSet *dest, PitchClassSet src) {
    if (src == NULL)
        return;
    pc_set_insert(dest, src->value);
    pc_set_insert_all(dest, src->left);
    pc_set_insert_all(dest, src->right);
}

PitchClassSet pc_set_union(PitchClassSet a, PitchClassSet b) {
    PitchClassSet result = pc_set_clone(a);
    pc_set_insert_all(&result, b);
    return result;
}

PitchClassSet pc_set_intersection(PitchClassSet a, PitchClassSet b) {
    if (a == NULL || b == NULL)
        return NULL;

    PitchClassSet result = NULL;
    if (pc_set_contains(b, a->value))
        pc_set_insert(&result, a->value);

    PitchClassSet left = pc_set_intersection(a->left, b);
    PitchClassSet right = pc_set_intersection(a->right, b);

    result = pc_set_union(result, left);
    result = pc_set_union(result, right);

    return result;
}

PitchClassSet pc_set_difference(PitchClassSet a, PitchClassSet b) {
    if (a == NULL)
        return NULL;

    PitchClassSet result = NULL;

    if (!pc_set_contains(b, a->value)) {
        pc_set_insert(&result, a->value);
    }

    // Recurse left and right
    PitchClassSet left = pc_set_difference(a->left, b);
    PitchClassSet right = pc_set_difference(a->right, b);

    // Merge results
    result = pc_set_union(result, left);
    result = pc_set_union(result, right);

    return result;
}
#endif // MEANTONAL

