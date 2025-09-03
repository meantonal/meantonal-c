#ifndef TYPES_H
#define TYPES_H

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
    int chroma_offset; // offset used by internal functions to reconcile notes.
} TonalContext;

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

typedef struct {
    enum Degree degree;
    enum Alteration alteration;
} TonalPitch;

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
 * This type is reserved for operations using Map2d matrices.
 */
typedef struct {
    int x;
    int y;
} MapVec;

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

#endif
