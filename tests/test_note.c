#include "../include/note.h"
#include "test_framework.h"

void test_note_chroma(void) {
    Note n;
    parse_spn("C4", &n);
    ASSERT_EQ(note_chroma(n), 0);
    parse_spn("C5", &n);
    ASSERT_EQ(note_chroma(n), 0);
    parse_spn("G4", &n);
    ASSERT_EQ(note_chroma(n), 1);
    parse_spn("F6", &n);
    ASSERT_EQ(note_chroma(n), -1);
    parse_spn("A#7", &n);
    ASSERT_EQ(note_chroma(n), 10);
    parse_spn("Gb-1", &n);
    ASSERT_EQ(note_chroma(n), -6);
}

void test_note_letter(void) {
    Note n;
    parse_spn("C4", &n);
    ASSERT_EQ(note_letter(n), 2);
    parse_spn("E4", &n);
    ASSERT_EQ(note_letter(n), 4);
    parse_spn("F4", &n);
    ASSERT_EQ(note_letter(n), 5);
    parse_spn("F#4", &n);
    ASSERT_EQ(note_letter(n), 5);
    parse_spn("F####4", &n);
    ASSERT_EQ(note_letter(n), 5);
    parse_spn("C7", &n);
    ASSERT_EQ(note_letter(n), 2);
}

void test_note_accidental(void) {
    Note n;
    parse_spn("C4", &n);
    ASSERT_EQ(note_accidental(n), 0);
    parse_spn("C#1", &n);
    ASSERT_EQ(note_accidental(n), 1);
    parse_spn("Cb2", &n);
    ASSERT_EQ(note_accidental(n), -1);
    parse_spn("C###3", &n);
    ASSERT_EQ(note_accidental(n), 3);
    parse_spn("Cbbbb5", &n);
    ASSERT_EQ(note_accidental(n), -4);
}

void test_midi_value(void) {
    Note n;
    parse_spn("C4", &n);
    ASSERT_EQ(midi_value(n), 60);
    parse_spn("E4", &n);
    ASSERT_EQ(midi_value(n), 64);
    parse_spn("F4", &n);
    ASSERT_EQ(midi_value(n), 65);
    parse_spn("F#4", &n);
    ASSERT_EQ(midi_value(n), 66);
    parse_spn("Fx4", &n);
    ASSERT_EQ(midi_value(n), 67);
    parse_spn("C7", &n);
    ASSERT_EQ(midi_value(n), 96);
}

void test_note_functions(void) {
    RUN_TESTS(test_note_chroma);
    RUN_TESTS(test_note_letter);
    RUN_TESTS(test_note_accidental);
    RUN_TESTS(test_midi_value);
}
