#include "../include/parse.h"
#include "../include/pitch.h"
#include "test_framework.h"

void test_pitch_chroma(void) {
    Pitch p;
    pitch_from_spn("C4", &p);
    ASSERT_EQ(pitch_chroma(p), 0);
    pitch_from_spn("C5", &p);
    ASSERT_EQ(pitch_chroma(p), 0);
    pitch_from_spn("G4", &p);
    ASSERT_EQ(pitch_chroma(p), 1);
    pitch_from_spn("F6", &p);
    ASSERT_EQ(pitch_chroma(p), -1);
    pitch_from_spn("A#7", &p);
    ASSERT_EQ(pitch_chroma(p), 10);
    pitch_from_spn("Gb-1", &p);
    ASSERT_EQ(pitch_chroma(p), -6);
}

void test_steps_between(void) {
    Pitch p;
    Pitch q;
    pitch_from_spn("C4", &p);
    pitch_from_spn("F#4", &q);
    ASSERT_EQ(steps_between(p, q), 3);
    pitch_from_spn("Ab3", &q);
    ASSERT_EQ(steps_between(p, q), -2);
    pitch_from_spn("C#4", &q);
    ASSERT_EQ(steps_between(p, q), 0);
}

void test_pitches_equal(void) {
    Pitch p = {0, 0};
    Pitch q;
    pitch_from_spn("C-1", &q);
    ASSERT_EQ(pitches_equal(p, q), true);
    pitch_from_spn("C4", &q);
    ASSERT_EQ(pitches_equal(p, q), false);
}

void test_pitches_enharmonic(void) {
    Pitch p, q;
    pitch_from_spn("C#4", &p);
    pitch_from_spn("Db4", &q);
    ASSERT_EQ(pitches_enharmonic(p, q, 12), true);
    ASSERT_EQ(pitches_enharmonic(p, q, 31), false);
    pitch_from_spn("Gbb4", &p);
    pitch_from_spn("Ex4", &q);
    ASSERT_EQ(pitches_enharmonic(p, q, 31), true);
    ASSERT_EQ(pitches_enharmonic(p, q, 12), false);
}

void test_pitch_midi(void) {
    Pitch p;
    pitch_from_spn("C4", &p);
    ASSERT_EQ(pitch_midi(p), 60);
    pitch_from_spn("E4", &p);
    ASSERT_EQ(pitch_midi(p), 64);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(pitch_midi(p), 65);
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(pitch_midi(p), 66);
    pitch_from_spn("Fx4", &p);
    ASSERT_EQ(pitch_midi(p), 67);
    pitch_from_spn("C7", &p);
    ASSERT_EQ(pitch_midi(p), 96);
}

void test_pitch_letter(void) {
    Pitch p;
    pitch_from_spn("C4", &p);
    ASSERT_EQ(pitch_letter(p), 2);
    pitch_from_spn("E4", &p);
    ASSERT_EQ(pitch_letter(p), 4);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(pitch_letter(p), 5);
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(pitch_letter(p), 5);
    pitch_from_spn("F####4", &p);
    ASSERT_EQ(pitch_letter(p), 5);
    pitch_from_spn("C7", &p);
    ASSERT_EQ(pitch_letter(p), 2);
}

void test_pitch_accidental(void) {
    Pitch p;
    pitch_from_spn("C4", &p);
    ASSERT_EQ(pitch_accidental(p), 0);
    pitch_from_spn("C#1", &p);
    ASSERT_EQ(pitch_accidental(p), 1);
    pitch_from_spn("Cb2", &p);
    ASSERT_EQ(pitch_accidental(p), -1);
    pitch_from_spn("C###3", &p);
    ASSERT_EQ(pitch_accidental(p), 3);
    pitch_from_spn("Cbbbb5", &p);
    ASSERT_EQ(pitch_accidental(p), -4);
}

void test_pitch_octave(void) {
    Pitch p;
    pitch_from_spn("C4", &p);
    ASSERT_EQ(pitch_octave(p), 4);
    pitch_from_spn("F#3", &p);
    ASSERT_EQ(pitch_octave(p), 3);
    pitch_from_spn("Abbbb9", &p);
    ASSERT_EQ(pitch_octave(p), 9);
    pitch_from_spn("E####6", &p);
    ASSERT_EQ(pitch_octave(p), 6);
    pitch_from_spn("G-9", &p);
    ASSERT_EQ(pitch_octave(p), -9);
}

void test_pitch_to_standard(void) {
    Pitch p;
    pitch_from_spn("C#4", &p);
    StandardPitch s = pitch_to_standard(p);
    ASSERT_EQ(s.letter, 2);
    ASSERT_EQ(s.accidental, 1);
    ASSERT_EQ(s.octave, 4);
}

void test_pitch_from_standard(void) {
    StandardPitch s = {.letter = 2, .accidental = 1, .octave = 4};
    Pitch p = pitch_from_standard(s);
    ASSERT_EQ(p.w, 21);
    ASSERT_EQ(p.h, 7);
}

void test_pitch_from_chroma(void) {
    Pitch p;
    p = pitch_from_chroma(0, -1);
    ASSERT_EQ(p.w, 0);
    ASSERT_EQ(p.h, 0);
    p = pitch_from_chroma(0, 4);
    ASSERT_EQ(p.w, 25);
    ASSERT_EQ(p.h, 10);
    p = pitch_from_chroma(7, 4);
    ASSERT_EQ(p.w, 26);
    ASSERT_EQ(p.h, 9);
    p = pitch_from_chroma(4, 4);
    ASSERT_EQ(p.w, 27);
    ASSERT_EQ(p.h, 10);
}

void test_pitch_invert(void) {
    MirrorAxis a;
    axis_from_spn("C4", "G4", &a);
    Pitch p, q;
    pitch_from_spn("D4", &p);
    p = pitch_invert(p, a);
    pitch_from_spn("F4", &q);
    ASSERT_EQ(pitches_equal(p, q), true);
    pitch_from_spn("Eb4", &p);
    p = pitch_invert(p, a);
    pitch_from_spn("E4", &q);
    ASSERT_EQ(pitches_equal(p, q), true);
    pitch_from_spn("Bb2", &p);
    p = pitch_invert(p, a);
    pitch_from_spn("A5", &q);
    ASSERT_EQ(pitches_equal(p, q), true);
}

void test_pitch_functions(void) {
    RUN_TESTS(test_pitch_chroma);
    RUN_TESTS(test_steps_between);
    RUN_TESTS(test_pitches_equal);
    RUN_TESTS(test_pitches_enharmonic);
    RUN_TESTS(test_pitch_midi);
    RUN_TESTS(test_pitch_letter);
    RUN_TESTS(test_pitch_accidental);
    RUN_TESTS(test_pitch_octave);
    RUN_TESTS(test_pitch_to_standard);
    RUN_TESTS(test_pitch_from_standard);
    RUN_TESTS(test_pitch_from_chroma);
    RUN_TESTS(test_pitch_invert);
}
