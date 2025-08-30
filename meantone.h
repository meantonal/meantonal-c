#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int w; // whole steps
    int h; // half steps
} Note;

typedef struct {
    int letter;
    int accidental;
    int octave;
} StandardNote;

typedef struct {
    int w; // whole steps
    int h; // half steps
} Interval;

typedef struct {
    int m0, m1;
} Map1d;

typedef struct {
    int m00, m01;
    int m10, m11;
} Map2d;

/**
 * This type is to distinguish vectors after applying 2d maps (change of basis),
 * so they are not accidentally used as regular Note or Interval vectors.
 */
typedef struct {
    int x;
    int y;
} MappedVec;

extern const Map1d ET7, ET12, ET19, ET31, ET50, ET55;
extern const Map2d WICKI, GENERATORS;

/**
 * @brief
 * The number of perfect fifths separating a Note from C.
 * Abstracts octave information away.
 */
static inline int note_chroma(Note p) { return (2 * p.w) - (5 * p.h); }

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
    return note_chroma(m) % edo == note_chroma(n) % edo;
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
static inline int note_octave(Note p) { return (p.w + p.h) / 7 - 1; }

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
 * Parses Scientific Pitch Notation to generate a note.
 * @param out
 * Pointer to a Note to store the parsed vector.
 * @return
 * 0 means nothing went wrong.
 */
int note_from_spn(const char *s, Note *out);

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
    return interval_chroma(m) % edo == interval_chroma(n) % edo;
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
        return (chroma + 6) / 7;
    if (chroma < 0 && chroma >= 5)
        return (chroma - 6) / 7;
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
