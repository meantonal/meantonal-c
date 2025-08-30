#include "../include/note.h"
#include "test_framework.h"

void test_note_chroma(void) {
    Note n;
    note_from_spn("C4", &n);
    ASSERT_EQ(note_chroma(n), 0);
    note_from_spn("C5", &n);
    ASSERT_EQ(note_chroma(n), 0);
    note_from_spn("G4", &n);
    ASSERT_EQ(note_chroma(n), 1);
    note_from_spn("F6", &n);
    ASSERT_EQ(note_chroma(n), -1);
    note_from_spn("A#7", &n);
    ASSERT_EQ(note_chroma(n), 10);
    note_from_spn("Gb-1", &n);
    ASSERT_EQ(note_chroma(n), -6);
}

void test_note_letter(void) {
    Note n;
    note_from_spn("C4", &n);
    ASSERT_EQ(note_letter(n), 2);
    note_from_spn("E4", &n);
    ASSERT_EQ(note_letter(n), 4);
    note_from_spn("F4", &n);
    ASSERT_EQ(note_letter(n), 5);
    note_from_spn("F#4", &n);
    ASSERT_EQ(note_letter(n), 5);
    note_from_spn("F####4", &n);
    ASSERT_EQ(note_letter(n), 5);
    note_from_spn("C7", &n);
    ASSERT_EQ(note_letter(n), 2);
}

void test_note_accidental(void) {
    Note n;
    note_from_spn("C4", &n);
    ASSERT_EQ(note_accidental(n), 0);
    note_from_spn("C#1", &n);
    ASSERT_EQ(note_accidental(n), 1);
    note_from_spn("Cb2", &n);
    ASSERT_EQ(note_accidental(n), -1);
    note_from_spn("C###3", &n);
    ASSERT_EQ(note_accidental(n), 3);
    note_from_spn("Cbbbb5", &n);
    ASSERT_EQ(note_accidental(n), -4);
}

void test_note_midi(void) {
    Note n;
    note_from_spn("C4", &n);
    ASSERT_EQ(note_midi(n), 60);
    note_from_spn("E4", &n);
    ASSERT_EQ(note_midi(n), 64);
    note_from_spn("F4", &n);
    ASSERT_EQ(note_midi(n), 65);
    note_from_spn("F#4", &n);
    ASSERT_EQ(note_midi(n), 66);
    note_from_spn("Fx4", &n);
    ASSERT_EQ(note_midi(n), 67);
    note_from_spn("C7", &n);
    ASSERT_EQ(note_midi(n), 96);
}

void test_note_functions(void) {
    RUN_TESTS(test_note_chroma);
    RUN_TESTS(test_note_letter);
    RUN_TESTS(test_note_accidental);
    RUN_TESTS(test_note_midi);
}
