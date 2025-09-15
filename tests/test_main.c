#include "test_framework.h"

void test_pitch_functions(void);
void test_interval_functions(void);
void test_tonality_functions(void);
void test_map_functions(void);
void test_parse_functions(void);
void test_pc_set_functions(void);

int main(void) {
    RUN_GROUP(test_pitch_functions);
    RUN_GROUP(test_interval_functions);
    RUN_GROUP(test_tonality_functions);
    RUN_GROUP(test_map_functions);
    RUN_GROUP(test_parse_functions);
    RUN_GROUP(test_pc_set_functions);

    TEST_RESULTS();
    return tests_failed != 0;
}
