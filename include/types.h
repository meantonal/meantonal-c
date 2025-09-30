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
    Map1D stepmap;
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

#endif
