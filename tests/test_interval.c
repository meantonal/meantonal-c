#include "../include/interval.h"
#include "../include/note.h"
#include "test_framework.h"

void test_interval_between(void) {
    Note p, q;
    parse_spn("C4", &p);
    parse_spn("E4", &q);
    ASSERT_EQ(interval_between(p, q).w, 2);
    ASSERT_EQ(interval_between(p, q).h, 0);
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
