#include "../include/interval.h"
#include "../include/note.h"
#include <stdbool.h>

const Interval major_ints[7] = {
    {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
};

int parse_interval(const char *s, Interval *out) {
    const char *p = s;

    // 1. sign
    bool negative = false;
    if (*p == '-') {
        negative = true;
        p++;
    }

    // 2. quality (unadjusted)
    int quality = 0;
    while (*p == 'P' || *p == 'p' || *p == 'M' || *p == 'm' || *p == 'b' ||
           *p == '#' || *p == 'A' || *p == 'a' || *p == 'D' || *p == 'd') {
        switch (*p) {
        case 'A':
        case 'a':
        case '#':
            quality++;
            break;
        case 'm':
        case 'b':
            quality--;
            break;
        case 'D':
        case 'd':
            if (quality == 0)
                quality -= 2;
            else
                quality--;
            break;
        }
        p++;
    }

    // 3. generic size / octave
    char *end;
    long generic = strtol(p, &end, 10) - 1;
    if (end == p) {
        return 1; // no digits found
    }
    int simple = generic % 7;
    int octave = generic / 7;

    // 4. construct output note
    out->w = major_ints[simple].w;
    out->h = major_ints[simple].h;

    // 5. adjust octave
    out->w += 5 * octave;
    out->h += 2 * octave;

    // 6. adjust quality
    if (quality < 0 && (simple == 0 || simple == 3 || simple == 4))
        quality++;
    out->w += quality;
    out->h -= quality;

    // 7. account for sign
    if (negative) {
        out->w = -out->w;
        out->h = -out->h;
    }

    return 0;
}

int interval_from_spn(const char *p_str, const char *q_str, Interval *out) {
    Note p, q;
    if (parse_spn(p_str, &p))
        return 1;
    if (parse_spn(q_str, &q))
        return 1;
    out->w = q.w - p.w;
    out->h = q.h - p.h;
    return 0;
}
