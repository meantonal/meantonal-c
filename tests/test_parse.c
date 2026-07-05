
#include "../include/parse.h"
#include "test_framework.h"
#include <stdio.h>

// Builds `count` copies of `unit` into dest, for constructing the expected
// output of tick-based notations (LilyPond octave marks, ABC accidentals,
// etc.) without hand-counting repeated characters.
static void build_repeated(char *dest, size_t destsize, const char *unit,
                            int count) {
    dest[0] = '\0';
    size_t pos = 0;
    for (int i = 0; i < count; i++)
        pos += (size_t)snprintf(dest + pos, destsize - pos, "%s", unit);
}

void test_pitch_from_spn(void) {
    Pitch p;
    pitch_from_spn("C-1", &p);
    ASSERT_EQ(p.w, 0);
    ASSERT_EQ(p.h, 0);
    pitch_from_spn("C0", &p);
    ASSERT_EQ(p.w, 5);
    ASSERT_EQ(p.h, 2);
    pitch_from_spn("C4", &p);
    ASSERT_EQ(p.w, 25);
    ASSERT_EQ(p.h, 10);
    pitch_from_spn("C#4", &p);
    ASSERT_EQ(p.w, 26);
    ASSERT_EQ(p.h, 9);
    pitch_from_spn("Cb4", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 11);
    pitch_from_spn("Dbb4", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 12);
}

void test_pitch_from_lily(void) {
    Pitch p;
    pitch_from_lily("c,,,,", &p);
    ASSERT_EQ(p.w, 0);
    ASSERT_EQ(p.h, 0);
    pitch_from_lily("c,,,", &p);
    ASSERT_EQ(p.w, 5);
    ASSERT_EQ(p.h, 2);
    pitch_from_lily("c'", &p);
    ASSERT_EQ(p.w, 25);
    ASSERT_EQ(p.h, 10);
    pitch_from_lily("cis'", &p);
    ASSERT_EQ(p.w, 26);
    ASSERT_EQ(p.h, 9);
    pitch_from_lily("ces'", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 11);
    pitch_from_lily("deses'", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 12);
}

void test_pitch_from_helmholtz(void) {
    Pitch p;
    pitch_from_helmholtz("C,,,", &p);
    ASSERT_EQ(p.w, 0);
    ASSERT_EQ(p.h, 0);
    pitch_from_helmholtz("C,,", &p);
    ASSERT_EQ(p.w, 5);
    ASSERT_EQ(p.h, 2);
    pitch_from_helmholtz("c'", &p);
    ASSERT_EQ(p.w, 25);
    ASSERT_EQ(p.h, 10);
    pitch_from_helmholtz("c#'", &p);
    ASSERT_EQ(p.w, 26);
    ASSERT_EQ(p.h, 9);
    pitch_from_helmholtz("cb'", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 11);
    pitch_from_helmholtz("dbb'", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 12);
}

void test_pitch_from_abc(void) {
    Pitch p;
    pitch_from_abc("C,,,,,", &p);
    ASSERT_EQ(p.w, 0);
    ASSERT_EQ(p.h, 0);
    pitch_from_abc("C,,,,", &p);
    ASSERT_EQ(p.w, 5);
    ASSERT_EQ(p.h, 2);
    pitch_from_abc("C", &p);
    ASSERT_EQ(p.w, 25);
    ASSERT_EQ(p.h, 10);
    pitch_from_abc("c", &p);
    ASSERT_EQ(p.w, 30);
    ASSERT_EQ(p.h, 12);
    pitch_from_abc("^c'", &p);
    ASSERT_EQ(p.w, 36);
    ASSERT_EQ(p.h, 13);
    pitch_from_abc("_c'", &p);
    ASSERT_EQ(p.w, 34);
    ASSERT_EQ(p.h, 15);
    pitch_from_abc("__d'", &p);
    ASSERT_EQ(p.w, 34);
    ASSERT_EQ(p.h, 16);
}

void test_pitch_spn(void) {
    Pitch p;
    char buf[8];
    bool flagged;

    pitch_from_spn("C4", &p);
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "C4");
    ASSERT_EQ(flagged, false);

    pitch_from_spn("C#4", &p);
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "C#4");
    ASSERT_EQ(flagged, false);

    pitch_from_spn("Cb4", &p);
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "Cb4");
    ASSERT_EQ(flagged, false);

    pitch_from_spn("Dbb4", &p);
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "Dbb4");
    ASSERT_EQ(flagged, false);

    // Adding n to w and subtracting n from h raises the accidental by n
    // while leaving the letter and octave untouched, since pitch_letter and
    // pitch_octave only depend on (w + h). Used below to reach accidentals
    // that can't be spelled directly as an SPN literal.

    // Double sharp uses the explicit "x" switch case, not the numeric
    // fallback.
    pitch_from_spn("C4", &p);
    p.w += 2;
    p.h -= 2;
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "Cx4");
    ASSERT_EQ(flagged, false);

    pitch_from_spn("C4", &p);
    p.w += 4;
    p.h -= 4;
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "C4#4");
    ASSERT_EQ(flagged, false); // exactly quadruple sharp: not flagged

    pitch_from_spn("C4", &p);
    p.w += 5;
    p.h -= 5;
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "C5#4");
    ASSERT_EQ(flagged, true); // beyond quadruple sharp: flagged, still rendered

    pitch_from_spn("C4", &p);
    p.w -= 5;
    p.h += 5;
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "C5b4");
    ASSERT_EQ(flagged, true);

    // Large enough that the name can't fit in an 8-byte buffer: falls back
    // to "ERR" instead of writing out of bounds.
    pitch_from_spn("C4", &p);
    p.w += 200000000;
    p.h -= 200000000;
    flagged = pitch_spn(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);
}

void test_pitch_lily(void) {
    Pitch p, base;
    char buf[16];
    char reps[64];
    char expected[64];
    bool flagged;

    pitch_from_lily("c,,,,", &p);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "c,,,,");
    ASSERT_EQ(flagged, false);

    pitch_from_lily("c'", &p);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "c'");
    ASSERT_EQ(flagged, false);

    pitch_from_lily("cis'", &p);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "cis'");
    ASSERT_EQ(flagged, false);

    pitch_from_lily("ces'", &p);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "ces'");
    ASSERT_EQ(flagged, false);

    pitch_from_lily("deses'", &p);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "deses'");
    ASSERT_EQ(flagged, false);

    // Base with a zero accidental and zero octave ticks, to isolate each
    // dimension below. Adding n to w and subtracting n from h raises the
    // accidental by n without touching the letter or octave, since those
    // only depend on (w + h).
    pitch_from_lily("c", &p);
    base = p;

    p = base;
    p.w += 4;
    p.h -= 4;
    build_repeated(reps, sizeof(reps), "is", 4);
    snprintf(expected, sizeof(expected), "c%s", reps);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, false); // exactly quadruple sharp: not flagged

    p = base;
    p.w += 5;
    p.h -= 5;
    build_repeated(reps, sizeof(reps), "is", 5);
    snprintf(expected, sizeof(expected), "c%s", reps);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, true); // beyond quadruple: flagged, still rendered

    p = base;
    p.w -= 5;
    p.h += 5;
    build_repeated(reps, sizeof(reps), "es", 5);
    snprintf(expected, sizeof(expected), "c%s", reps);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, true);

    // One "is" past what 16 bytes can hold: falls back to "ERR".
    p = base;
    p.w += 8;
    p.h -= 8;
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    // An absurdly large accidental must still just overflow immediately,
    // not hang trying to build a hundred-million-character string.
    p = base;
    p.w += 100000000;
    p.h -= 100000000;
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    // Octave ticks alone can also overflow the buffer, independent of the
    // accidental-based flag (which stays false here since the accidental is
    // untouched). Adding (5, 2) transposes up an octave without changing
    // the accidental.
    p = base;
    p.w += 5 * 14;
    p.h += 2 * 14;
    build_repeated(reps, sizeof(reps), "'", 14);
    snprintf(expected, sizeof(expected), "c%s", reps);
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, false);

    p = base;
    p.w += 5 * 15;
    p.h += 2 * 15;
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    p = base;
    p.w += 5 * 100000000;
    p.h += 2 * 100000000;
    flagged = pitch_lily(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);
}

void test_pitch_helmholtz(void) {
    Pitch p, base;
    char buf[16];
    char expected[64];
    bool flagged;

    pitch_from_helmholtz("C,,,", &p);
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "C,,,");
    ASSERT_EQ(flagged, false);

    pitch_from_helmholtz("c'", &p);
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "c'");
    ASSERT_EQ(flagged, false);

    pitch_from_helmholtz("c#'", &p);
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "c#'");
    ASSERT_EQ(flagged, false);

    pitch_from_helmholtz("cb'", &p);
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "cb'");
    ASSERT_EQ(flagged, false);

    pitch_from_helmholtz("dbb'", &p);
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "dbb'");
    ASSERT_EQ(flagged, false);

    // Base with a zero accidental and zero octave ticks, to isolate each
    // dimension below.
    pitch_from_helmholtz("c", &p);
    base = p;

    p = base;
    p.w += 4;
    p.h -= 4;
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "c4#");
    ASSERT_EQ(flagged, false); // exactly quadruple sharp: not flagged

    p = base;
    p.w += 5;
    p.h -= 5;
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "c5#");
    ASSERT_EQ(flagged, true); // beyond quadruple: flagged, still rendered

    p = base;
    p.w -= 5;
    p.h += 5;
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "c5b");
    ASSERT_EQ(flagged, true);

    // The accidental is rendered as digits, not repeated characters, so
    // even a hundred-million-sharp accidental still fits in 16 bytes and
    // renders in full instead of hitting "ERR".
    p = base;
    p.w += 100000000;
    p.h -= 100000000;
    snprintf(expected, sizeof(expected), "c%d#", 100000000);
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, true);

    // The octave, however, is still rendered as repeated '/, characters,
    // so it can overflow the buffer on its own, independent of the
    // accidental-based flag (which stays false here).
    p = base;
    p.w += 5 * 14;
    p.h += 2 * 14;
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "c''''''''''''''");
    ASSERT_EQ(flagged, false);

    p = base;
    p.w += 5 * 15;
    p.h += 2 * 15;
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    // Must not hang trying to build a hundred-million-tick octave suffix.
    p = base;
    p.w += 5 * 100000000;
    p.h += 2 * 100000000;
    flagged = pitch_helmholtz(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);
}

void test_pitch_abc(void) {
    Pitch p, base;
    char buf[16];
    char reps[64];
    char expected[64];
    bool flagged;

    pitch_from_abc("C,,,,,", &p);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "C,,,,,");
    ASSERT_EQ(flagged, false);

    pitch_from_abc("C", &p);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "C");
    ASSERT_EQ(flagged, false);

    pitch_from_abc("c", &p);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "c");
    ASSERT_EQ(flagged, false);

    pitch_from_abc("^c'", &p);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "^c'");
    ASSERT_EQ(flagged, false);

    pitch_from_abc("_c'", &p);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "_c'");
    ASSERT_EQ(flagged, false);

    pitch_from_abc("__d'", &p);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "__d'");
    ASSERT_EQ(flagged, false);

    // Base with a zero accidental and zero octave ticks, to isolate each
    // dimension below.
    pitch_from_abc("c", &p);
    base = p;

    p = base;
    p.w += 4;
    p.h -= 4;
    build_repeated(reps, sizeof(reps), "^", 4);
    snprintf(expected, sizeof(expected), "%sc", reps);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, false); // exactly quadruple sharp: not flagged

    p = base;
    p.w += 5;
    p.h -= 5;
    build_repeated(reps, sizeof(reps), "^", 5);
    snprintf(expected, sizeof(expected), "%sc", reps);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, true); // beyond quadruple: flagged, still rendered

    p = base;
    p.w -= 5;
    p.h += 5;
    build_repeated(reps, sizeof(reps), "_", 5);
    snprintf(expected, sizeof(expected), "%sc", reps);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, true);

    // Both the accidental and octave are rendered as repeated characters
    // in ABC notation, so either can overflow the buffer on its own.
    p = base;
    p.w += 15;
    p.h -= 15;
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    p = base;
    p.w += 100000000;
    p.h -= 100000000;
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    p = base;
    p.w += 5 * 14;
    p.h += 2 * 14;
    build_repeated(reps, sizeof(reps), "'", 14);
    snprintf(expected, sizeof(expected), "c%s", reps);
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, expected);
    ASSERT_EQ(flagged, false);

    p = base;
    p.w += 5 * 15;
    p.h += 2 * 15;
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);

    p = base;
    p.w += 5 * 100000000;
    p.h += 2 * 100000000;
    flagged = pitch_abc(p, buf);
    ASSERT_STR_EQ(buf, "ERR");
    ASSERT_EQ(flagged, true);
}

void test_parse_functions(void) {
    RUN_TESTS(test_pitch_from_spn);
    RUN_TESTS(test_pitch_from_lily);
    RUN_TESTS(test_pitch_from_helmholtz);
    RUN_TESTS(test_pitch_from_abc);
    RUN_TESTS(test_pitch_spn);
    RUN_TESTS(test_pitch_lily);
    RUN_TESTS(test_pitch_helmholtz);
    RUN_TESTS(test_pitch_abc);
}
