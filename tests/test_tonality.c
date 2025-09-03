#include "../include/tonality.h"
#include "test_framework.h"

void test_context_from_str(void) {
    TonalContext k;
    context_from_str("F#", DORIAN, &k);
    ASSERT_EQ(k.tonic.letter, 5);
    ASSERT_EQ(k.tonic.accidental, 1);
    ASSERT_EQ(k.chroma_offset, -3);
    ASSERT_EQ(k.mode, DORIAN);
    context_from_str("Eb", MAJOR, &k);
    ASSERT_EQ(k.chroma_offset, 4);
    ASSERT_EQ(k.mode, MAJOR);
}

void test_degree_number(void) {
    TonalContext k;
    context_from_str("F#", DORIAN, &k);
    Pitch p;
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(degree_number(p, k), 0);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(degree_number(p, k), 0);
    pitch_from_spn("C4", &p);
    ASSERT_EQ(degree_number(p, k), 4);
    pitch_from_spn("C-3", &p);
    ASSERT_EQ(degree_number(p, k), 4);
    pitch_from_spn("D10", &p);
    ASSERT_EQ(degree_number(p, k), 5);
}

void test_degree_alteration(void) {
    TonalContext k;
    context_from_str("F#", DORIAN, &k);
    Pitch p;
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(degree_alteration(p, k), DIATONIC_DEG);
    pitch_from_spn("Fx4", &p);
    ASSERT_EQ(degree_alteration(p, k), RAISED_DEG);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(degree_alteration(p, k), LOWERED_DEG);
    pitch_from_spn("Dx4", &p);
    ASSERT_EQ(degree_alteration(p, k), FOREIGN_DEG_SHARP);
    pitch_from_spn("Db4", &p);
    ASSERT_EQ(degree_alteration(p, k), FOREIGN_DEG_FLAT);
}

void test_degree_chroma(void) {
    TonalContext k;
    context_from_str("C", MAJOR, &k);
    ASSERT_EQ(degree_chroma(TONIC, k), 0);
    ASSERT_EQ(degree_chroma(SUPERTONIC, k), 2);
    ASSERT_EQ(degree_chroma(MEDIANT, k), 4);
    ASSERT_EQ(degree_chroma(SUBDOMINANT, k), -1);
    ASSERT_EQ(degree_chroma(DOMINANT, k), 1);
    ASSERT_EQ(degree_chroma(SUBMEDIANT, k), 3);
    ASSERT_EQ(degree_chroma(SUBTONIC, k), 5);
    context_from_str("C", DORIAN, &k);
    ASSERT_EQ(degree_chroma(TONIC, k), 0);
    ASSERT_EQ(degree_chroma(SUPERTONIC, k), 2);
    ASSERT_EQ(degree_chroma(MEDIANT, k), -3);
    ASSERT_EQ(degree_chroma(SUBDOMINANT, k), -1);
    ASSERT_EQ(degree_chroma(DOMINANT, k), 1);
    ASSERT_EQ(degree_chroma(SUBMEDIANT, k), 3);
    ASSERT_EQ(degree_chroma(SUBTONIC, k), -2);
    context_from_str("F#", PHRYGIAN, &k);
    ASSERT_EQ(degree_chroma(TONIC, k), 6);
    ASSERT_EQ(degree_chroma(SUPERTONIC, k), 1);
    ASSERT_EQ(degree_chroma(MEDIANT, k), 3);
    ASSERT_EQ(degree_chroma(SUBDOMINANT, k), 5);
    ASSERT_EQ(degree_chroma(DOMINANT, k), 7);
    ASSERT_EQ(degree_chroma(SUBMEDIANT, k), 2);
    ASSERT_EQ(degree_chroma(SUBTONIC, k), 4);
}

void test_key_functions(void) {
    RUN_TESTS(test_context_from_str);
    RUN_TESTS(test_degree_number);
    RUN_TESTS(test_degree_alteration);
    RUN_TESTS(test_degree_chroma);
}
