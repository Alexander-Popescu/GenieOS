#include "test.h"
#include "serial.h"

void runSuite(TEST_CONTEXT *ctx, char *suiteName, TEST_CASE *tests, int count) {
    serialWriteString(suiteName);
    serialWriteString(": ");

    int failuresBefore = ctx->failed;

    for (int i = 0; i < count; i++) {
        bool result = tests[i].run();
        ctx->run++;

        if (result) {
            ctx->passed++;
        } else {
            ctx->failed++;
            if (ctx->failed - failuresBefore == 1) {
                serialWriteChar('\n');
            }
            serialWriteString("  FAIL: ");
            serialWriteString(tests[i].name);
            serialWriteChar('\n');
        }
    }

    if (ctx->failed == failuresBefore) {
        serialWriteString("OK\n");
    }
}

void testSummary(TEST_CONTEXT *ctx) {
    serialWriteString("Result: ");
    serialWriteInt(ctx->passed);
    serialWriteString(" passed, ");
    serialWriteInt(ctx->failed);
    serialWriteString(" failed, ");
    serialWriteInt(ctx->run);
    serialWriteString(" total\n");
}
