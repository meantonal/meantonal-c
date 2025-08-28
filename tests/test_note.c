#include "../include/note.h"
#include "test_framework.h"

void test_note_chroma(void) {}

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
    RUN_TESTS(test_note_letter);
    RUN_TESTS(test_midi_value);
}
