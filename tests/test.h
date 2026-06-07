#ifndef TEST_H
#define TEST_H

#include "../util/intdef.h"

typedef struct {
    char *name;
    bool (*run)(void);
} TEST_CASE;

typedef struct {
    int run;
    int passed;
    int failed;
} TEST_CONTEXT;

void runSuite(TEST_CONTEXT *ctx, char *suiteName, TEST_CASE *tests, int count);
void testSummary(TEST_CONTEXT *ctx);

#endif
