#include "../drivers/VGAText.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../apps/shell.h"
#include "../tests/test_runner.h"

void systemInit() {
    initScreen();
    initISR();
    initKeyboard();
    init_timer(100);

    // application initializations
    shellInit();
    asm volatile("sti");
}

void main() {
    runTests();
    systemInit();
    for (;;) {
        asm volatile("hlt");
    }
}
