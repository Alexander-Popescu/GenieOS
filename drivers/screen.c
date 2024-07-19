#include "screen.h"
#include "ports.h"

int getCursorOffset()
{
    int offset;
    //tell VGA to place high byte of cursor into data register
    port_byte_out(VGA_CTRL_REG, 14);
    offset += (port_byte_in(VGA_DATA_REG) << 8);
    
    //now low byte 
    port_byte_out(VGA_CTRL_REG, 15);
    offset += port_byte_in(VGA_DATA_REG);

    return offset * 2; 

}

void setCursorOffset(int offset)
{
    offset /= 2;

    port_byte_out(VGA_CTRL_REG, 14);
    port_byte_out(VGA_DATA_REG, (unsigned char)(offset >> 8));
    port_byte_out(VGA_CTRL_REG, 15);
    port_byte_out(VGA_DATA_REG, (unsigned char)(offset & 0xFF));
}

int printChar(char c, int col, int row, int attribute)
{
    int offset;
    if (attribute == 0) {attribute = WHITE_ON_BLACK;}

    if (col < 0 || row < 0)
    {
	offset = getCursorOffset();
    } else {
	offset = 2 * (MAX_COLS * (row) + col);
    }

    if (c == '\n') {
	row = (offset / MAX_COLS) / 2;
	offset = 2 * (row + 1 * MAX_COLS);
    } else {
	unsigned char *vRam = (unsigned char*) VIDEO_ADDRESS;
	vRam[offset] = c;
	vRam[offset + 1] = attribute;
	offset += 2;
    }
    setCursorOffset(offset);
    return offset;
}

void clear_screen() {
    unsigned char* vRam = (unsigned char*)VIDEO_ADDRESS;
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++)
    {
	vRam[2 * i] = ' ';
	vRam[2 * i + 1] = WHITE_ON_BLACK;
    }
    setCursorOffset(0);
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }

void kprint_at(char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = getCursorOffset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = printChar(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char *message) {
    kprint_at(message, -1, -1);
}
