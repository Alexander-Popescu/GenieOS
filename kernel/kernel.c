#include "../drivers/VGAText.h"
#include "../cpu/isr.h"

void systemInit() {
    initScreen();
    initISR();
}

int main() {
    systemInit();
    printString("Welcome to GenieOS\n", -1, 0);
    asm volatile ("int $0");
    asm volatile ("int $2");
    asm volatile ("int $255");
}
