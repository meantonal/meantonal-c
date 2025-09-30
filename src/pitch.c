#include "../include/pitch.h"
#include "../include/interval.h"
#include "../include/map.h"

Pitch pitch_from_chroma(int chroma, int octave) {
    Pitch p = {0, 0};
    p.w += chroma * 3;
    p.h += chroma;
    while (pitch_octave(p) > octave) {
        p.w -= 5;
        p.h -= 2;
    }
    while (pitch_octave(p) < octave) {
        p.w += 5;
        p.h += 2;
    }
    return p;
}

Pitch pitch_highest(Pitch arr[], int len, TuningMap T) {
    Pitch highest = arr[0];
    for (int i = 1; i < len; i++) {
        if (to_hz(arr[i], T) > to_hz(highest, T))
            highest = arr[i];
        else if (to_hz(arr[i], T) == to_hz(highest, T) &&
                 steps_between(arr[i], highest) < 0)
            highest = arr[i];
    }
    return highest;
}

Pitch pitch_lowest(Pitch arr[], int len, TuningMap T) {
    Pitch lowest = arr[0];
    for (int i = 1; i < len; i++) {
        if (to_hz(arr[i], T) < to_hz(lowest, T))
            lowest = arr[i];
        else if (to_hz(arr[i], T) == to_hz(lowest, T) &&
                 steps_between(arr[i], lowest) > 0)
            lowest = arr[i];
    }
    return lowest;
}

double oriented_ratio_between(Pitch p, Pitch q, TuningMap T) {
    double x = to_ratio(interval_between(p, q), T);
    double y = to_ratio(interval_between(q, p), T);
    return x >= y ? x : y;
}

Pitch pitch_nearest(Pitch p, Pitch arr[], int len, TuningMap T) {
    double smallest_ratio = oriented_ratio_between(p, arr[0], T);

    for (int i = 1; i < len; i++) {
        double x = oriented_ratio_between(p, arr[i], T);
        if (x < smallest_ratio)
            smallest_ratio = x;
    }

    Pitch candidates[len];
    int candidates_len = 0;
    for (int i = 0; i < len; i++) {
        if (oriented_ratio_between(p, arr[i], T) == smallest_ratio)
            candidates[candidates_len++] = arr[i];
    }

    Pitch closest = candidates[0];
    for (int i = 1; i < candidates_len; i++) {
        if (abs(steps_between(p, candidates[i])) <
            abs(steps_between(p, closest)))
            closest = candidates[i];
    }

    return closest;
}
