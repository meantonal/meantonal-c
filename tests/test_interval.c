#include "../include/interval.h"
#include "../include/parse.h"
#include "../include/pitch.h"
#include "test_framework.h"
#include <stdbool.h>

void assert_int_between(char *p_str, char *q_str, char *m_str) {
    Pitch p, q;
    Interval m, n;
    pitch_from_spn(p_str, &p);
    pitch_from_spn(q_str, &q);
    interval_from_name(m_str, &m);
    n = interval_between(p, q);
    ASSERT_EQ(m.w, n.w);
    ASSERT_EQ(m.h, n.h);
}

void test_interval_between(void) {
    assert_int_between("C4", "E4", "M3");
    assert_int_between("C5", "E4", "-m6");
    assert_int_between("C5", "A#5", "A6");
    assert_int_between("C3", "G4", "P12");
    assert_int_between("B3", "Ab4", "d7");
    assert_int_between("Eb5", "B4", "-d4");
}

void test_interval_chroma(void) {
    Interval n;
    interval_from_name("P8", &n);
    ASSERT_EQ(interval_chroma(n), 0);
    interval_from_name("A6", &n);
    ASSERT_EQ(interval_chroma(n), 10);
    interval_from_name("m3", &n);
    ASSERT_EQ(interval_chroma(n), -3);
}

void test_intervals_equal(void) {
    Interval m = {3, 1};
    Interval n;
    interval_from_name("P5", &n);
    ASSERT_EQ(intervals_equal(m, n), true);
    interval_from_name("M3", &n);
    ASSERT_EQ(intervals_equal(m, n), false);
}

void test_intervals_enharmonic(void) {
    Interval m, n;
    interval_from_name("A6", &m);
    interval_from_name("m7", &n);
    ASSERT_EQ(intervals_enharmonic(m, n, 12), true);
    ASSERT_EQ(intervals_enharmonic(m, n, 31), false);
    interval_from_name("dddd10", &m);
    interval_from_name("P8", &n);
    ASSERT_EQ(intervals_enharmonic(m, n, 31), true);
    ASSERT_EQ(intervals_enharmonic(m, n, 12), false);
}

void test_stepspan(void) {
    Interval m;
    interval_from_spn("C4", "E4", &m);
    ASSERT_EQ(stepspan(m), 2);
    interval_from_spn("Ab4", "F#5", &m);
    ASSERT_EQ(stepspan(m), 5);
    interval_from_spn("Db4", "Ab3", &m);
    ASSERT_EQ(stepspan(m), -3);
}

void assert_int_quality(char *m_str, int expected) {
    Interval m;
    interval_from_name(m_str, &m);
    ASSERT_EQ(interval_quality(m), expected);
}

void test_interval_quality(void) {
    assert_int_quality("P1", 0);
    assert_int_quality("P4", 0);
    assert_int_quality("P5", 0);

    assert_int_quality("M2", 1);
    assert_int_quality("M3", 1);
    assert_int_quality("M6", 1);
    assert_int_quality("M7", 1);
    assert_int_quality("m2", -1);
    assert_int_quality("m3", -1);
    assert_int_quality("m6", -1);
    assert_int_quality("m7", -1);

    assert_int_quality("A4", 2);
    assert_int_quality("A7", 2);
    assert_int_quality("d4", -2);
    assert_int_quality("d7", -2);

    assert_int_quality("AAAA2", 5);
    assert_int_quality("dddd2", -5);
}

void test_transpose_real(void) {
    Pitch p, q;
    Interval m;
    pitch_from_spn("C4", &p);
    pitch_from_spn("E4", &q);
    interval_from_name("M3", &m);
    p = transpose_real(p, m);
    ASSERT_EQ(pitches_equal(p, q), true);
}

void test_interval_from_name(void) {
    Interval m;
    interval_from_name("A6", &m);
    Interval n = {5, 0};
    ASSERT_EQ(intervals_equal(m, n), true);
}

void test_interval_from_spn(void) {
    Interval m;
    interval_from_spn("B3", "Ab4", &m);
    Interval n = {3, 3};
    ASSERT_EQ(intervals_equal(m, n), true);
}

void test_interval_functions(void) {
    RUN_TESTS(test_interval_between);
    RUN_TESTS(test_interval_chroma);
    RUN_TESTS(test_intervals_equal);
    RUN_TESTS(test_intervals_enharmonic);
    RUN_TESTS(test_stepspan);
    RUN_TESTS(test_interval_quality);
    RUN_TESTS(test_transpose_real);
    RUN_TESTS(test_interval_from_name);
    RUN_TESTS(test_interval_from_spn);
}
