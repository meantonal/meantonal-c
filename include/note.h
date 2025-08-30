#ifndef NOTE_H
#define NOTE_H

#include "types.h"
#include <stdbool.h>

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

#endif
