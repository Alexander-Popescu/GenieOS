#include "../drivers/VGAText.h"


void test() {
    //does nothing
}

int main() {
    initScreen();
    printString("Test1!", MAX_COLS * 3, 0);
    printString("Test2", -1, 0);
    printString("\nTest3", -1,0);
    printString("Testing Screen\n Scroll!", OFFSET_LIMIT - 2, 0);
}
