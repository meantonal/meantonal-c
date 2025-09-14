#include "../include/map.h"
#include "../include/constants.h"
#include "../include/interval.h"
#include "../include/types.h"
#include <math.h>

TuningMap tuning_map_from_fifth(double fifth, Pitch ref_pitch,
                                double ref_freq) {
    return (TuningMap){.ref_pitch = ref_pitch,
                       .ref_freq = ref_freq,
                       .centmap = (Map1D){fifth, 1200}};
}

TuningMap tuning_map_from_edo(int edo, Pitch ref_pitch, double ref_freq) {
    int fifth_steps = round(log2(1.5) * edo);
    double fifth = (float)fifth_steps * 1200 / edo;
    return tuning_map_from_fifth(fifth, ref_pitch, ref_freq);
}

double to_cents(Interval m, TuningMap T) {
    return map_to_1d(map_to_2d((MapVec){m.w, m.h}, GENERATORS_TO), T.centmap);
}

double to_ratio(Interval m, TuningMap T) {
    return pow(2, to_cents(m, T) / 1200);
}

double to_hz(Pitch p, TuningMap T) {
    return T.ref_freq * to_ratio(interval_between(T.ref_pitch, p), T);
}
