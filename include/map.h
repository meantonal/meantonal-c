#ifndef MAP_H
#define MAP_H

#include "types.h"

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

/**
 * Returns an ordered pitch numbering for the passed Pitch as an integer.
 * Available in any EDO TuningMap created via TuningMap.fromEDO. For 12TET, this
 * will be the ordinary MIDI value for a given Pitch, but for other EDO tunings
 * it provides an ordered MIDI-equvalent mapping.
 */
int to_pitch_number(Pitch p, TuningMap T);

#endif
