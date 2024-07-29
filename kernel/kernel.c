#include "../drivers/VGAText.h"


void test() {
    //does nothing
}

int main() {
    initScreen();
    printString("Testing Screen Scroll!", OFFSET_LIMIT - 3, 0);
}
