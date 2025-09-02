#include "../include/key.h"
#include "test_framework.h"

void test_key_from_str(void) {
    Key k;
    key_from_str("F#", DORIAN, &k);
    ASSERT_EQ(k.chroma, 6);
    ASSERT_EQ(k.mode, DORIAN);
    key_from_str("Eb", MAJOR, &k);
    ASSERT_EQ(k.chroma, -3);
    ASSERT_EQ(k.mode, MAJOR);
}

void test_degree_number(void) {
    Key k;
    key_from_str("F#", DORIAN, &k);
    Pitch p;
    pitch_from_spn("F#4", &p);
    ASSERT_EQ(degree_number(p, k), 0);
    pitch_from_spn("F4", &p);
    ASSERT_EQ(degree_number(p, k), 0);
    pitch_from_spn("C4", &p);
    ASSERT_EQ(degree_number(p, k), 4);
}

void test_degree_alteration(void) {
    Key k;
    key_from_str("F#", DORIAN, &k);
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

void test_key_functions(void) {
    RUN_TESTS(test_key_from_str);
    RUN_TESTS(test_degree_number);
    RUN_TESTS(test_degree_alteration);
}
