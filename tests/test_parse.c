
#include "../include/parse.h"
#include "test_framework.h"

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

void test_parse_functions(void) {
    RUN_TESTS(test_pitch_from_spn);
    RUN_TESTS(test_pitch_from_lily);
    RUN_TESTS(test_pitch_from_helmholtz);
    RUN_TESTS(test_pitch_from_abc);
}
