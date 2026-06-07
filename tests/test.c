#include "test.h"
#include "serial.h"

void runSuite(TEST_CONTEXT *ctx, char *suiteName, TEST_CASE *tests, int count) {
    serialWriteString("--- ");
    serialWriteString(suiteName);
    serialWriteString(" ---\n");

    for (int i = 0; i < count; i++) {
        bool result = tests[i].run();
        ctx->run++;

        if (result) {
            ctx->passed++;
            serialWriteString("  [PASS] ");
        } else {
            ctx->failed++;
            serialWriteString("  [FAIL] ");
        }
        serialWriteString(tests[i].name);
        serialWriteChar('\n');
    }
}

void testSummary(TEST_CONTEXT *ctx) {
    serialWriteString("==============================\n");
    serialWriteString("  ");
    serialWriteInt(ctx->passed);
    serialWriteString(" passed, ");
    serialWriteInt(ctx->failed);
    serialWriteString(" failed, ");
    serialWriteInt(ctx->run);
    serialWriteString(" total\n");
    serialWriteString("==============================\n");
}
