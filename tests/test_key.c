#include "../include/key.h"
#include "test_framework.h"

void test_key_from_str(void) {
    Key k;
    key_from_str("F#", DORIAN, &k);
    ASSERT_EQ(k.chroma, 7);
    ASSERT_EQ(k.mode, DORIAN);
}

void test_key_functions(void) { RUN_TESTS(test_key_from_str); }
