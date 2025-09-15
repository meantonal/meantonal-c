#include "../include/pitch.h"

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
