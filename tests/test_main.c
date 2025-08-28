#include "test_framework.h"

void test_note_functions(void);

int main(void) {
    RUN_GROUP(test_note_functions);

    TEST_RESULTS();
    return tests_failed != 0;
}
