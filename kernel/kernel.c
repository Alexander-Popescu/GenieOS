#include "../drivers/ports.h"
#include "../drivers/screen.h"

void main() {
    clear_screen();
    kprint_at("Test!\nTest2!", 0, 0);
}
