#include "../drivers/ports.h"

int main() {
    //ask VGA control register (0x3d4) for high byte of cursor (14)
    port_byte_out(0x3d4, 14);
    //data is stored in data register (0x3d5)
    int position = port_byte_in(0x3d5);
    position = position << 8;

    //now low byte (15)
    port_byte_out(0x3d4, 15);
    position += port_byte_in(0x3d5);

    int vga_offset = position * 2; //vga stores another word for color info on each character slot

    char *vga = (char *)0xB8000;
    vga[vga_offset] = 'X';
    vga[vga_offset + 1] = 0x0F; //color


}
