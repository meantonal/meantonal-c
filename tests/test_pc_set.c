#include "../include/pc_set.h"
#include "test_framework.h"

void test_pc_set(void) {
    PitchClassSet s = NULL;
    s = pc_set_insert(s, 1);
    s = pc_set_insert(s, 2);
    s = pc_set_insert(s, 5);
    s = pc_set_insert(s, 6);
    ASSERT_EQ(pc_set_contains(s, 1), true);
    ASSERT_EQ(pc_set_contains(s, 2), true);
    ASSERT_EQ(pc_set_contains(s, 5), true);
    ASSERT_EQ(pc_set_contains(s, 6), true);
    ASSERT_EQ(pc_set_contains(s, 0), false);
    ASSERT_EQ(pc_set_contains(s, 3), false);
    ASSERT_EQ(pc_set_contains(s, 4), false);
    pc_set_destroy(s);
}

void test_pc_set_functions(void) { RUN_TESTS(test_pc_set); }
