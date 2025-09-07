#include "../include/pc_set.h"
#include "test_framework.h"

void test_pc_set_insert(void) {
    PitchClassSet s = NULL;
    pc_set_insert(&s, 1);
    pc_set_insert(&s, 2);
    pc_set_insert(&s, 5);
    pc_set_insert(&s, 6);
    ASSERT_EQ(pc_set_contains(s, 1), true);
    ASSERT_EQ(pc_set_contains(s, 2), true);
    ASSERT_EQ(pc_set_contains(s, 5), true);
    ASSERT_EQ(pc_set_contains(s, 6), true);
    ASSERT_EQ(pc_set_contains(s, 0), false);
    ASSERT_EQ(pc_set_contains(s, 3), false);
    ASSERT_EQ(pc_set_contains(s, 4), false);
    pc_set_destroy(s);
}

void test_pc_set_transpose(void) {
    PitchClassSet s = NULL;
    pc_set_insert(&s, 1);
    pc_set_insert(&s, 2);
    pc_set_insert(&s, 5);
    pc_set_insert(&s, 6);
    s = pc_set_transpose(s, 3);
    ASSERT_EQ(pc_set_contains(s, 4), true);
    ASSERT_EQ(pc_set_contains(s, 5), true);
    ASSERT_EQ(pc_set_contains(s, 8), true);
    ASSERT_EQ(pc_set_contains(s, 9), true);
    ASSERT_EQ(pc_set_contains(s, 1), false);
}

void test_pc_set_invert(void) {
    PitchClassSet s = NULL;
    pc_set_insert(&s, 1);
    pc_set_insert(&s, 2);
    pc_set_insert(&s, 5);
    pc_set_insert(&s, 6);
    MirrorAxis a = axis_create((Pitch){0, 0}, (Pitch){3, 1});
    s = pc_set_invert(s, pitch_chroma((Pitch)a));
    ASSERT_EQ(pc_set_contains(s, 0), true);
    ASSERT_EQ(pc_set_contains(s, -1), true);
    ASSERT_EQ(pc_set_contains(s, -4), true);
    ASSERT_EQ(pc_set_contains(s, -5), true);
    ASSERT_EQ(pc_set_contains(s, 1), false);
}

void test_pc_set_union(void) {
    PitchClassSet s = NULL, t = NULL;
    pc_set_insert(&s, 1);
    pc_set_insert(&s, 2);
    pc_set_insert(&s, 3);
    pc_set_insert(&t, 4);
    pc_set_insert(&t, 5);
    pc_set_insert(&t, 6);
    s = pc_set_union(s, t);
    ASSERT_EQ(pc_set_contains(s, 1), true);
    ASSERT_EQ(pc_set_contains(s, 2), true);
    ASSERT_EQ(pc_set_contains(s, 3), true);
    ASSERT_EQ(pc_set_contains(s, 4), true);
    ASSERT_EQ(pc_set_contains(s, 5), true);
    ASSERT_EQ(pc_set_contains(s, 6), true);
    ASSERT_EQ(pc_set_contains(s, 7), false);
}

void test_pc_set_intersection(void) {
    PitchClassSet s = NULL, t = NULL;
    pc_set_insert(&s, 1);
    pc_set_insert(&s, 2);
    pc_set_insert(&s, 3);
    pc_set_insert(&t, 2);
    pc_set_insert(&t, 3);
    pc_set_insert(&t, 4);
    s = pc_set_intersection(s, t);
    ASSERT_EQ(pc_set_contains(s, 1), false);
    ASSERT_EQ(pc_set_contains(s, 2), true);
    ASSERT_EQ(pc_set_contains(s, 3), true);
    ASSERT_EQ(pc_set_contains(s, 4), false);
}

void test_pc_set_difference(void) {
    PitchClassSet s = NULL, t = NULL;
    pc_set_insert(&s, 1);
    pc_set_insert(&s, 2);
    pc_set_insert(&s, 3);
    pc_set_insert(&t, 2);
    pc_set_insert(&t, 3);
    pc_set_insert(&t, 4);
    s = pc_set_difference(s, t);
    ASSERT_EQ(pc_set_contains(s, 1), true);
    ASSERT_EQ(pc_set_contains(s, 2), false);
    ASSERT_EQ(pc_set_contains(s, 3), false);
    ASSERT_EQ(pc_set_contains(s, 4), false);
}

void test_pc_set_functions(void) {
    RUN_TESTS(test_pc_set_insert);
    RUN_TESTS(test_pc_set_transpose);
    RUN_TESTS(test_pc_set_invert);
    RUN_TESTS(test_pc_set_union);
    RUN_TESTS(test_pc_set_intersection);
    RUN_TESTS(test_pc_set_difference);
}
