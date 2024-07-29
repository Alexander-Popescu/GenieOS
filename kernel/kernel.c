#include "../drivers/VGAText.h"


void test() {
    //does nothing
}

int main() {
    initScreen();
    printString("Test1!", 0, 0);
    printString("Test2!", MAX_COLS, 0);
    printString("Test3!", -1, 0);
    scrollScreen();
}
