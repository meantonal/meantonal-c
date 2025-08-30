#ifndef TYPES_H
#define TYPES_H

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

#endif
