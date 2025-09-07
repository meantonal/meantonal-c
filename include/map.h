#ifndef MAP_H
#define MAP_H

#include "types.h"

/**
 * Maps to an integer using a 1x2 matrix.
 * Most built-in functions that take Pitches and return integers perform
 * this operation somewhere along the way.
 */
static inline int map_to_1d(MapVec p, Map1d T) {
    return T.m0 * p.w + T.m1 * p.h;
}

/**
 * Maps to a 2d MapVec type using a 2x2 matrix.
 * You must cast the result to a Pitch or Interval if you intend to use it as
 * one.
 */
static inline MapVec map_to_2d(MapVec p, Map2d T) {
    return (MapVec){.w = T.m00 * p.w + T.m01 * p.h,
                    .h = T.m10 * p.w + T.m11 * p.h};
}

#endif
