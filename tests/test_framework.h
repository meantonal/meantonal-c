#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>

extern int tests_run;
extern int tests_failed;

#define ASSERT_EQ(actual, expected)                                            \
    do {                                                                       \
        tests_run++;                                                           \
        int a = (actual);                                                      \
        int e = (expected);                                                    \
        if (a != e) {                                                          \
            tests_failed++;                                                    \
            fprintf(stderr, "    ❌ FAIL: %s:%d: got %d, expected %d\n",       \
                    __FILE__, __LINE__, a, e);                                 \
        }                                                                      \
    } while (0)

#define RUN_TESTS(testfunc)                                                    \
    do {                                                                       \
        fprintf(stderr, "    Running %s...\n", #testfunc);                     \
        testfunc();                                                            \
    } while (0)

#define RUN_GROUP(testfunc)                                                    \
    do {                                                                       \
        fprintf(stderr, "Running %s...\n", #testfunc);                         \
        testfunc();                                                            \
    } while (0)

#define TEST_RESULTS()                                                         \
    do {                                                                       \
        if (tests_failed == 0) {                                               \
            printf("All %d tests passed ✅\n", tests_run);                     \
        } else {                                                               \
            printf("%d/%d tests failed ❌\n", tests_failed, tests_run);        \
        }                                                                      \
    } while (0)

#endif
