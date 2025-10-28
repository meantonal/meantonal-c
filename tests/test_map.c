#include "../include/constants.h"
#include "../include/map.h"
#include "../include/parse.h"
#include "../include/pitch.h"
#include "test_framework.h"

void test_map_to_2d(void) {
    MapVec u = {1, 0};
    MapVec v = map_to_2d(u, WICKI_TO);
    ASSERT_EQ(v.x, 1);
    ASSERT_EQ(v.y, 0);
    u = (MapVec){3, 1};
    v = map_to_2d(u, WICKI_TO);
    ASSERT_EQ(v.x, 0);
    ASSERT_EQ(v.y, 1);
}

void test_map_to_1d(void) {
    MapVec v = {5, 3};
    ASSERT_EQ(map_to_1d(v, (Map1D){5, 5}), 40);
}

void test_pitch_to_number(void) {
    Pitch p;
    pitch_from_spn("C4", &p);

    Pitch ref;
    pitch_from_spn("A4", &ref);
    EDOMap T = create_edo_map(12);
    ASSERT_EQ(pitch_to_number(p, T), 60);

    T = create_edo_map(31);
    ASSERT_EQ(pitch_to_number(p, T), 155);
}

void test_pitches_compare(void) {
    Pitch p;
    pitch_from_spn("C#4", &p);

    Pitch q;
    pitch_from_spn("Db4", &q);
    EDOMap T = create_edo_map(12);
    ASSERT_EQ(pitches_compare(p, q, T), 0);

    T = create_edo_map(31);
    ASSERT_EQ(pitches_compare(p, q, T), -1);

    T = create_edo_map(53);
    ASSERT_EQ(pitches_compare(p, q, T), 1);
}

void test_map_functions(void) {
    RUN_TESTS(test_map_to_2d);
    RUN_TESTS(test_map_to_1d);
    RUN_TESTS(test_pitch_to_number);
    RUN_TESTS(test_pitches_compare);
}
