#ifndef INTERVAL_H
#define INTERVAL_H

#include "note.h"
#include "stdbool.h"

typedef struct {
    int w; // whole steps
    int h; // half steps
} Interval;

static inline bool same_interval(Interval m, Interval n) {
    return (m.w == n.w && m.h == n.h);
}

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

int parse_interval(const char *s, Interval *out);
int interval_from_spn(const char *p_str, const char *q_str, Interval *out);

#endif
