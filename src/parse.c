#include "../include/pitch.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>

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

void pitch_spn(Pitch p, char *out) {
    size_t pos = 0;
    size_t cap = 8;

    char letter = pitch_letter(p) + 'A';
    int accidental = pitch_accidental(p);
    int octave = pitch_octave(p);

    pos += snprintf(out + pos, cap - pos, "%c", letter);

    switch (accidental) {
    case 2:
        pos += snprintf(out + pos, cap - pos, "x");
        break;
    case 1:
        pos += snprintf(out + pos, cap - pos, "#");
        break;
    case 0:
        break;
    case -1:
        pos += snprintf(out + pos, cap - pos, "b");
        break;
    case -2:
        pos += snprintf(out + pos, cap - pos, "bb");
        break;
    default:
        if (accidental > 0) {
            pos += snprintf(out + pos, cap - pos, "%d#", accidental);
        } else {
            pos += snprintf(out + pos, cap - pos, "%db", -accidental);
        }
        break;
    }
    pos += snprintf(out + pos, cap - pos, "%d", octave);
}
