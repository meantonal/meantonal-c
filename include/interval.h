#ifndef INTERVAL_H
#define INTERVAL_H

#include "note.h"

typedef struct {
    int w; // whole steps
    int h; // half steps
} Interval;

/**
 * @return Interval q - p
 * */
static inline Interval interval_between(Note p, Note q) {
    return (Interval){.w = q.w - p.w, .h = q.h - p.h};
}

/**
 * Get the number of diatonic steps subtended by an Interval.
 * */
static inline int stepspan(Interval m) { return m.w + m.h; }

#endif
