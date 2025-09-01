#include "test_framework.h"

void test_note_functions(void);
void test_interval_functions(void);
void test_key_functions(void);

int main(void) {
    RUN_GROUP(test_note_functions);
    RUN_GROUP(test_interval_functions);
    RUN_GROUP(test_key_functions);

    TEST_RESULTS();
    return tests_failed != 0;
}
