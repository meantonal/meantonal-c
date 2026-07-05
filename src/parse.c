#include "../include/pitch.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const Pitch letters[7] = {
    {4, 1}, {5, 1}, {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1},
};

Pitch pitch_from_standard(StandardPitch p) {
    return (Pitch){
        .w = letters[p.letter].w + 5 * p.octave + p.accidental,
        .h = letters[p.letter].h + 2 * p.octave - p.accidental,
    };
}

int pitch_from_spn(const char *s, Pitch *out) {
    const char *p = s;

    int letter;
    if (*p >= 'A' && *p <= 'G') {
        letter = *p++ - 'A';
    } else if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w = letters[letter].w;
    out->h = letters[letter].h;

    int acc = 0;
    while (*p == '#' || *p == 'b' || *p == 'x' || *p == 'w') {
        switch (*p) {
        case '#':
            acc++;
            break;
        case 'b':
            acc--;
            break;
        case 'x':
            acc += 2;
            break;
        case 'w':
            acc -= 2;
            break;
        }
        p++;
    }
    out->w += acc;
    out->h -= acc;

    char *end;
    long oct = strtol(p, &end, 10) + 1;
    if (end == p) {
        return 1; // no digits found
    }
    out->w += (int)oct * 5;
    out->h += (int)oct * 2;

    return 0;
}

int pitch_from_lily(const char *s, Pitch *out) {
    const char *p = s;

    int letter;
    if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w = letters[letter].w;
    out->h = letters[letter].h;

    int acc = 0;
    while (*p == 'i' || *p == 'e') {
        switch (*p) {
        case 'i':
            acc++;
            break;
        case 'e':
            acc--;
            break;
        }
        p += 2;
    }
    out->w += acc;
    out->h -= acc;

    int oct = 4;
    while (*p == '\'' || *p == ',') {
        switch (*p) {
        case '\'':
            oct++;
            break;
        case ',':
            oct--;
            break;
        }
        p++;
    }
    out->w += oct * 5;
    out->h += oct * 2;

    return 0;
}

int pitch_from_helmholtz(const char *s, Pitch *out) {
    const char *p = s;

    int letter;
    int oct = 4;
    if (*p >= 'A' && *p <= 'G') {
        letter = *p++ - 'A';
        oct--;
    } else if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w = letters[letter].w;
    out->h = letters[letter].h;

    int acc = 0;
    while (*p == '#' || *p == 'b' || *p == 'x' || *p == 'w') {
        switch (*p) {
        case '#':
            acc++;
            break;
        case 'b':
            acc--;
            break;
        case 'x':
            acc += 2;
            break;
        case 'w':
            acc -= 2;
            break;
        }
        p++;
    }
    out->w += acc;
    out->h -= acc;

    while (*p == '\'' || *p == ',') {
        switch (*p) {
        case '\'':
            oct++;
            break;
        case ',':
            oct--;
            break;
        }
        p++;
    }
    out->w += oct * 5;
    out->h += oct * 2;

    return 0;
}

int pitch_from_abc(const char *s, Pitch *out) {
    const char *p = s;

    int acc = 0;
    while (*p == '^' || *p == '=' || *p == '_') {
        switch (*p) {
        case '^':
            acc++;
            break;
        case '_':
            acc--;
            break;
        }
        p++;
    }
    out->w = acc;
    out->h = -acc;

    int letter;
    int oct = 6;
    if (*p >= 'A' && *p <= 'G') {
        letter = *p++ - 'A';
        oct--;
    } else if (*p >= 'a' && *p <= 'g') {
        letter = *p++ - 'a';
    } else {
        return 1; // invalid
    }
    out->w += letters[letter].w;
    out->h += letters[letter].h;

    while (*p == '\'' || *p == ',') {
        switch (*p) {
        case '\'':
            oct++;
            break;
        case ',':
            oct--;
            break;
        }
        p++;
    }
    out->w += oct * 5;
    out->h += oct * 2;

    return 0;
}

bool pitch_spn(Pitch p, char *out) {
    // Sized so that no individual snprintf call below can ever truncate:
    // 1 (letter) + 11 (accidental, worst case "-2147483648b") + 11 (octave,
    // worst case "-2147483648") + 1 (nul) = 24, with margin to spare.
    char tmp[32];
    size_t pos = 0;

    char letter = pitch_letter(p) + 'A';
    int accidental = pitch_accidental(p);
    int octave = pitch_octave(p);
    bool flagged = accidental > 4 || accidental < -4;

    pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%c", letter);

    switch (accidental) {
    case 2:
        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "x");
        break;
    case 1:
        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "#");
        break;
    case 0:
        break;
    case -1:
        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "b");
        break;
    case -2:
        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "bb");
        break;
    default:
        if (accidental > 0) {
            pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%d#", accidental);
        } else {
            // Cast to avoid UB negating INT_MIN.
            pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%lldb",
                             -(long long)accidental);
        }
        break;
    }
    pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%d", octave);

    if (pos >= 8) {
        snprintf(out, 8, "ERR");
        return true;
    }

    snprintf(out, 8, "%s", tmp);
    return flagged;
}

bool pitch_lily(Pitch p, char *out) {
    size_t cap = 16;

    char letter = pitch_letter(p) + 'a';
    int accidental = pitch_accidental(p);
    int octave = pitch_octave(p) - 3;
    bool flagged = accidental > 4 || accidental < -4;

    // "is"/"es" cost 2 chars per unit of accidental, "'"/"," cost 1 char per
    // unit of octave. Compute the total length up front (rather than
    // looping to find out) so a huge vector can never turn into a
    // multi-billion-iteration loop.
    long long accidental_mag =
        accidental < 0 ? -(long long)accidental : accidental;
    long long octave_mag = octave < 0 ? -(long long)octave : octave;
    long long needed = 1 + 2 * accidental_mag + octave_mag;

    if (needed + 1 > (long long)cap) {
        snprintf(out, cap, "ERR");
        return true;
    }

    size_t pos = 0;
    pos += snprintf(out + pos, cap - pos, "%c", letter);
    while (accidental) {
        if (accidental > 0) {
            pos += snprintf(out + pos, cap - pos, "is");
            accidental--;
        } else {
            pos += snprintf(out + pos, cap - pos, "es");
            accidental++;
        }
    }

    while (octave) {
        if (octave > 0) {
            pos += snprintf(out + pos, cap - pos, "'");
            octave--;
        } else {
            pos += snprintf(out + pos, cap - pos, ",");
            octave++;
        }
    }
    return flagged;
}

bool pitch_helmholtz(Pitch p, char *out) {
    size_t cap = 16;

    char letter = pitch_letter(p);
    int accidental = pitch_accidental(p);
    bool flagged = accidental > 4 || accidental < -4;

    int octave = pitch_octave(p) - 3;
    if (octave < 0) {
        octave++;
        letter += 'A';
    } else
        letter += 'a';

    // The accidental is rendered as text (bounded regardless of magnitude,
    // same as pitch_spn), but the octave is rendered as repeated '/,
    // (linear in magnitude), so its length has to be checked before
    // looping.
    char accidental_str[32];
    switch (accidental) {
    case 2:
        snprintf(accidental_str, sizeof(accidental_str), "x");
        break;
    case 1:
        snprintf(accidental_str, sizeof(accidental_str), "#");
        break;
    case 0:
        accidental_str[0] = '\0';
        break;
    case -1:
        snprintf(accidental_str, sizeof(accidental_str), "b");
        break;
    case -2:
        snprintf(accidental_str, sizeof(accidental_str), "bb");
        break;
    default:
        if (accidental > 0) {
            snprintf(accidental_str, sizeof(accidental_str), "%d#",
                     accidental);
        } else {
            // Cast to avoid UB negating INT_MIN.
            snprintf(accidental_str, sizeof(accidental_str), "%lldb",
                     -(long long)accidental);
        }
        break;
    }

    long long octave_mag = octave < 0 ? -(long long)octave : octave;
    long long needed = 1 + (long long)strlen(accidental_str) + octave_mag;

    if (needed + 1 > (long long)cap) {
        snprintf(out, cap, "ERR");
        return true;
    }

    size_t pos = 0;
    pos += snprintf(out + pos, cap - pos, "%c", letter);
    pos += snprintf(out + pos, cap - pos, "%s", accidental_str);

    while (octave) {
        if (octave > 0) {
            pos += snprintf(out + pos, cap - pos, "'");
            octave--;
        } else {
            pos += snprintf(out + pos, cap - pos, ",");
            octave++;
        }
    }
    return flagged;
}

bool pitch_abc(Pitch p, char *out) {
    size_t cap = 16;

    char letter = pitch_letter(p);
    int accidental = pitch_accidental(p);
    bool flagged = accidental > 4 || accidental < -4;

    int octave = pitch_octave(p) - 5;
    if (octave < 0) {
        octave++;
        letter += 'A';
    } else
        letter += 'a';

    // Both the accidental ("^"/"_") and octave ("'"/",") are rendered as
    // repeated characters, linear in magnitude, so the total length has to
    // be checked before looping.
    long long accidental_mag =
        accidental < 0 ? -(long long)accidental : accidental;
    long long octave_mag = octave < 0 ? -(long long)octave : octave;
    long long needed = accidental_mag + 1 + octave_mag;

    if (needed + 1 > (long long)cap) {
        snprintf(out, cap, "ERR");
        return true;
    }

    size_t pos = 0;
    while (accidental) {
        if (accidental > 0) {
            pos += snprintf(out + pos, cap - pos, "^");
            accidental--;
        } else {
            pos += snprintf(out + pos, cap - pos, "_");
            accidental++;
        }
    }

    pos += snprintf(out + pos, cap - pos, "%c", letter);

    while (octave) {
        if (octave > 0) {
            pos += snprintf(out + pos, cap - pos, "'");
            octave--;
        } else {
            pos += snprintf(out + pos, cap - pos, ",");
            octave++;
        }
    }
    return flagged;
}
