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
    EDOMap T;
    create_edo_map(12, &T);
    ASSERT_EQ(pitch_to_number(p, T), 60);

    create_edo_map(31, &T);
    ASSERT_EQ(pitch_to_number(p, T), 155);
}

void test_pitches_compare(void) {
    Pitch p;
    pitch_from_spn("C#4", &p);

    Pitch q;
    pitch_from_spn("Db4", &q);
    EDOMap T;
    create_edo_map(12, &T);
    ASSERT_EQ(pitches_compare(p, q, T), 0);

    create_edo_map(31, &T);
    ASSERT_EQ(pitches_compare(p, q, T), -1);

    create_edo_map(53, &T);
    ASSERT_EQ(pitches_compare(p, q, T), 1);
}

void test_create_edo_map_rejects_bad_edo(void) {
    EDOMap T;
    ASSERT_EQ(create_edo_map(12, &T), 0);
    ASSERT_EQ(create_edo_map(19, &T), 0);
    ASSERT_EQ(create_edo_map(31, &T), 0);
    // 5-EDO (720¢) and 7-EDO (~685.7¢) sit exactly on the boundaries.
    ASSERT_EQ(create_edo_map(5, &T), 0);
    ASSERT_EQ(create_edo_map(7, &T), 0);
    // 3-EDO (800¢) and 4-EDO (600¢) fall outside the range.
    ASSERT_EQ(create_edo_map(3, &T), 1);
    ASSERT_EQ(create_edo_map(4, &T), 1);
}

void test_tuning_map_from_fifth_rejects_bad_fifth(void) {
    Pitch ref;
    pitch_from_spn("A4", &ref);
    TuningMap T;

    ASSERT_EQ(tuning_map_from_fifth(700, ref, 440, &T), 0);
    // Boundaries are inclusive.
    ASSERT_EQ(tuning_map_from_fifth(1200.0 * 4 / 7, ref, 440, &T), 0);
    ASSERT_EQ(tuning_map_from_fifth(1200.0 * 3 / 5, ref, 440, &T), 0);
    // 600¢ (4-EDO's fifth): too narrow for a diatonic.
    ASSERT_EQ(tuning_map_from_fifth(600, ref, 440, &T), 1);
    // 800¢ (3-EDO's fifth): too wide for a diatonic.
    ASSERT_EQ(tuning_map_from_fifth(800, ref, 440, &T), 1);
}

void test_tuning_map_from_edo_rejects_bad_edo(void) {
    Pitch ref;
    pitch_from_spn("A4", &ref);
    TuningMap T;

    ASSERT_EQ(tuning_map_from_edo(12, ref, 440, &T), 0);
    ASSERT_EQ(tuning_map_from_edo(19, ref, 440, &T), 0);
    ASSERT_EQ(tuning_map_from_edo(31, ref, 440, &T), 0);
    ASSERT_EQ(tuning_map_from_edo(5, ref, 440, &T), 0);
    ASSERT_EQ(tuning_map_from_edo(7, ref, 440, &T), 0);
    ASSERT_EQ(tuning_map_from_edo(3, ref, 440, &T), 1);
    ASSERT_EQ(tuning_map_from_edo(4, ref, 440, &T), 1);
}

void test_map_functions(void) {
    RUN_TESTS(test_map_to_2d);
    RUN_TESTS(test_map_to_1d);
    RUN_TESTS(test_pitch_to_number);
    RUN_TESTS(test_pitches_compare);
    RUN_TESTS(test_create_edo_map_rejects_bad_edo);
    RUN_TESTS(test_tuning_map_from_fifth_rejects_bad_fifth);
    RUN_TESTS(test_tuning_map_from_edo_rejects_bad_edo);
}
