#include "../include/constants.h"
#include "../include/note.h"
#include "test_framework.h"

void test_note_chroma(void) {
    Note p;
    note_from_spn("C4", &p);
    ASSERT_EQ(note_chroma(p), 0);
    note_from_spn("C5", &p);
    ASSERT_EQ(note_chroma(p), 0);
    note_from_spn("G4", &p);
    ASSERT_EQ(note_chroma(p), 1);
    note_from_spn("F6", &p);
    ASSERT_EQ(note_chroma(p), -1);
    note_from_spn("A#7", &p);
    ASSERT_EQ(note_chroma(p), 10);
    note_from_spn("Gb-1", &p);
    ASSERT_EQ(note_chroma(p), -6);
}

void test_notes_equal(void) {
    Note p = {0, 0};
    Note q;
    note_from_spn("C-1", &q);
    ASSERT_EQ(notes_equal(p, q), true);
    note_from_spn("C4", &q);
    ASSERT_EQ(notes_equal(p, q), false);
}

void test_notes_enharmonic(void) {
    Note p, q;
    note_from_spn("C#4", &p);
    note_from_spn("Db4", &q);
    ASSERT_EQ(notes_enharmonic(p, q, 12), true);
    ASSERT_EQ(notes_enharmonic(p, q, 31), false);
    note_from_spn("Gbb4", &p);
    note_from_spn("Ex4", &q);
    ASSERT_EQ(notes_enharmonic(p, q, 31), true);
    ASSERT_EQ(notes_enharmonic(p, q, 12), false);
}

void test_note_midi(void) {
    Note p;
    note_from_spn("C4", &p);
    ASSERT_EQ(note_midi(p), 60);
    note_from_spn("E4", &p);
    ASSERT_EQ(note_midi(p), 64);
    note_from_spn("F4", &p);
    ASSERT_EQ(note_midi(p), 65);
    note_from_spn("F#4", &p);
    ASSERT_EQ(note_midi(p), 66);
    note_from_spn("Fx4", &p);
    ASSERT_EQ(note_midi(p), 67);
    note_from_spn("C7", &p);
    ASSERT_EQ(note_midi(p), 96);
}

void test_note_letter(void) {
    Note p;
    note_from_spn("C4", &p);
    ASSERT_EQ(note_letter(p), 2);
    note_from_spn("E4", &p);
    ASSERT_EQ(note_letter(p), 4);
    note_from_spn("F4", &p);
    ASSERT_EQ(note_letter(p), 5);
    note_from_spn("F#4", &p);
    ASSERT_EQ(note_letter(p), 5);
    note_from_spn("F####4", &p);
    ASSERT_EQ(note_letter(p), 5);
    note_from_spn("C7", &p);
    ASSERT_EQ(note_letter(p), 2);
}

void test_note_accidental(void) {
    Note p;
    note_from_spn("C4", &p);
    ASSERT_EQ(note_accidental(p), 0);
    note_from_spn("C#1", &p);
    ASSERT_EQ(note_accidental(p), 1);
    note_from_spn("Cb2", &p);
    ASSERT_EQ(note_accidental(p), -1);
    note_from_spn("C###3", &p);
    ASSERT_EQ(note_accidental(p), 3);
    note_from_spn("Cbbbb5", &p);
    ASSERT_EQ(note_accidental(p), -4);
}

void test_note_octave(void) {
    Note p;
    note_from_spn("C4", &p);
    ASSERT_EQ(note_octave(p), 4);
    note_from_spn("F#3", &p);
    ASSERT_EQ(note_octave(p), 3);
    note_from_spn("Abbbb9", &p);
    ASSERT_EQ(note_octave(p), 9);
    note_from_spn("E####6", &p);
    ASSERT_EQ(note_octave(p), 6);
    note_from_spn("G-9", &p);
    ASSERT_EQ(note_octave(p), -9);
}

void test_note_to_standard(void) {
    Note p;
    note_from_spn("C#4", &p);
    StandardNote s = note_to_standard(p);
    ASSERT_EQ(s.letter, 2);
    ASSERT_EQ(s.accidental, 1);
    ASSERT_EQ(s.octave, 4);
}

void test_note_map_2d(void) {
    Note p = {1, 0};
    MappedVec v = note_map_2d(p, WICKI_TO);
    ASSERT_EQ(v.x, 1);
    ASSERT_EQ(v.y, 0);
    p = (Note){3, 1};
    v = note_map_2d(p, WICKI_TO);
    ASSERT_EQ(v.x, 0);
    ASSERT_EQ(v.y, 1);
}

void test_note_map_1d(void) {
    Note p = {5, 3};
    ASSERT_EQ(note_map_1d(p, (Map1d){5, 5}), 40);
}

void test_note_from_standard(void) {
    StandardNote s = {.letter = 2, .accidental = 1, .octave = 4};
    Note p = note_from_standard(s);
    ASSERT_EQ(p.w, 21);
    ASSERT_EQ(p.h, 7);
}

void test_note_from_spn(void) {
    Note p;
    note_from_spn("C-1", &p);
    ASSERT_EQ(p.w, 0);
    ASSERT_EQ(p.h, 0);
    note_from_spn("C0", &p);
    ASSERT_EQ(p.w, 5);
    ASSERT_EQ(p.h, 2);
    note_from_spn("C4", &p);
    ASSERT_EQ(p.w, 25);
    ASSERT_EQ(p.h, 10);
    note_from_spn("C#4", &p);
    ASSERT_EQ(p.w, 26);
    ASSERT_EQ(p.h, 9);
    note_from_spn("Cb4", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 11);
    note_from_spn("Dbb4", &p);
    ASSERT_EQ(p.w, 24);
    ASSERT_EQ(p.h, 12);
}

void test_note_invert(void) {
    MirrorAxis a;
    axis_from_spn("C4", "G4", &a);
    Note p, q;
    note_from_spn("D4", &p);
    p = note_invert(p, a);
    note_from_spn("F4", &q);
    ASSERT_EQ(notes_equal(p, q), true);
    note_from_spn("Eb4", &p);
    p = note_invert(p, a);
    note_from_spn("E4", &q);
    ASSERT_EQ(notes_equal(p, q), true);
    note_from_spn("Bb2", &p);
    p = note_invert(p, a);
    note_from_spn("A5", &q);
    ASSERT_EQ(notes_equal(p, q), true);
}

void test_note_functions(void) {
    RUN_TESTS(test_note_chroma);
    RUN_TESTS(test_notes_equal);
    RUN_TESTS(test_notes_enharmonic);
    RUN_TESTS(test_note_midi);
    RUN_TESTS(test_note_letter);
    RUN_TESTS(test_note_accidental);
    RUN_TESTS(test_note_octave);
    RUN_TESTS(test_note_to_standard);
    RUN_TESTS(test_note_map_2d);
    RUN_TESTS(test_note_map_1d);
    RUN_TESTS(test_note_from_standard);
    RUN_TESTS(test_note_from_spn);
    RUN_TESTS(test_note_invert);
}
