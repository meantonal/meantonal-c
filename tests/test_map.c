#include "../include/constants.h"
#include "../include/map.h"
#include "test_framework.h"

void test_map_to_2d(void) {
    MapVec u = {1, 0};
    MapVec v = map_to_2d(u, WICKI_TO);
    ASSERT_EQ(v.w, 1);
    ASSERT_EQ(v.h, 0);
    u = (MapVec){3, 1};
    v = map_to_2d(u, WICKI_TO);
    ASSERT_EQ(v.w, 0);
    ASSERT_EQ(v.h, 1);
}

void test_map_to_1d(void) {
    MapVec v = {5, 3};
    ASSERT_EQ(map_to_1d(v, (Map1d){5, 5}), 40);
}

void test_map_functions(void) {
    RUN_TESTS(test_map_to_2d);
    RUN_TESTS(test_map_to_1d);
}
