#ifndef NOTE_H
#define NOTE_H

#include <stdlib.h>

typedef struct {
    int w; // whole steps
    int h; // half steps
} Note;

/**
 * The number of perfect fifths separating a Note from C.
 * Abstracts octave information away.
 * */
static inline int note_chroma(Note p) { return (2 * p.w) - (5 * p.h); }

/**
 * Get the letter number of a Note.
 * To convert to an actual letter, just add 'a' or 'A'.
 * */
static inline int note_letter(Note p) { return (p.w + p.h + 2) % 7; }

/**
 * 0 is natural.
 * Sharps are positive.
 * Flats are negative.
 * */
static inline int note_accidental(Note p) {
    int chroma = note_chroma(p) - 2;
    int accidental = (abs(chroma) + 3) / 7;

    return chroma < 0 ? -accidental : accidental;
}

static inline int midi_value(Note p) { return 2 * p.w + p.h; }

int parse_spn(const char *s, Note *out_note);

#endif
