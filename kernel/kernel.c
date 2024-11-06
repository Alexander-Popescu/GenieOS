#include "../drivers/VGAText.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../apps/shell.h"

void systemInit() {
    initScreen();
    initISR();
    initKeyboard();

    // application initializations
    shellInit();
    asm volatile("sti");
}

int main() {
    systemInit();
}
