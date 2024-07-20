#include "../drivers/ports.h"
#include "../drivers/screen.h"

void main() {
    clearScreen();

    /* Fill up the screen */
    int i = 0;
    for (i = 0; i < 24; i++) {
        char str[255];
        int_to_ascii(i, str);
        VGAPrintPos(str, 0, i);
    }

    VGAPrintPos("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
    VGAPrint("And with this text, the kernel will scroll again, and row 1 will disappear too!");
}
