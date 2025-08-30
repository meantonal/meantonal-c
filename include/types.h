#ifndef TYPES_H
#define TYPES_H

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
 * isomorphic keyboard layou
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
} NoteAxis;

#endif
