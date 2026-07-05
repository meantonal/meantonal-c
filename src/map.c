#include "../include/map.h"
#include "../include/constants.h"
#include "../include/interval.h"
#include "../include/types.h"
#include <math.h>

// A fifth outside this range can't produce a well-defined diatonic scale.
static int fifth_supports_diatonic(double fifth) {
    return fifth >= (1200.0 * 4 / 7) && fifth <= (1200.0 * 3 / 5);
}

int tuning_map_from_fifth(double fifth, Pitch ref_pitch, double ref_freq,
                          TuningMap *out) {
    if (!fifth_supports_diatonic(fifth))
        return 1;
    *out = (TuningMap){.ref_pitch = ref_pitch,
                       .ref_freq = ref_freq,
                       .centmap = (Map1D){fifth, 1200}};
    return 0;
}

int tuning_map_from_edo(int edo, Pitch ref_pitch, double ref_freq,
                       TuningMap *out) {
    int fifth_steps = round(log2(1.5) * edo);
    double fifth = (float)fifth_steps * 1200 / edo;
    return tuning_map_from_fifth(fifth, ref_pitch, ref_freq, out);
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

int create_edo_map(int edo, EDOMap *out) {
    int fifth_steps = (int)round(log2(1.5) * edo);
    double fifth = (double)fifth_steps * 1200 / edo;
    if (!fifth_supports_diatonic(fifth))
        return 1;

    int whole = ((fifth_steps * 2) % edo + edo) % edo;
    int half = ((fifth_steps * -5) % edo + edo) % edo;
    *out = (EDOMap){whole, half};
    return 0;
}
