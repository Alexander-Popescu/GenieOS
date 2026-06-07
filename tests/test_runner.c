#include "test_runner.h"
#include "test.h"
#include "serial.h"
#include "test_util.h"

typedef struct {
    char *name;
    void (*run)(TEST_CONTEXT *ctx);
} TEST_SUITE;

void runTests() {
    TEST_CONTEXT ctx = {0, 0, 0};
    initSerial();

    serialWriteString("==============================\n");
    serialWriteString("  GenieOS Unit Tests\n");
    serialWriteString("==============================\n");

    TEST_SUITE suites[] = {
        {"util", testUtilSuite},
    };
    int suiteCount = sizeof(suites) / sizeof(suites[0]);

    for (int i = 0; i < suiteCount; i++) {
        suites[i].run(&ctx);
    }

    testSummary(&ctx);
}
