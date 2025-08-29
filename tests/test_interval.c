#include "../include/interval.h"
#include "../include/note.h"
#include "test_framework.h"
#include <stdbool.h>

void assert_int_between(char *p_str, char *q_str, char *m_str) {
    Note p, q;
    Interval m;
    Interval n;
    parse_spn(p_str, &p);
    parse_spn(q_str, &q);
    parse_interval(m_str, &m);
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

void test_stepspan(void) {
    Interval m;
    interval_from_spn("C4", "E4", &m);
    ASSERT_EQ(stepspan(m), 2);
    interval_from_spn("Ab4", "F#5", &m);
    ASSERT_EQ(stepspan(m), 5);
    interval_from_spn("Db4", "Ab3", &m);
    ASSERT_EQ(stepspan(m), -3);
}

void test_interval_functions(void) {
    RUN_TESTS(test_interval_between);
    RUN_TESTS(test_stepspan);
}
