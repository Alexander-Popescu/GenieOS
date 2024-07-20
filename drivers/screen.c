#include "screen.h"
#include "ports.h"

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }

int getCursorOffset() {
    int offset;

    port_byte_out(VGA_CTRL_REG, 14);
    offset = port_byte_in(VGA_DATA_REG) << 8;
    port_byte_out(VGA_CTRL_REG, 15);
    offset += port_byte_in(VGA_DATA_REG);

    return offset * 2;
}

void setCursorOffset(int offset) {
    offset /= 2;

    port_byte_out(VGA_CTRL_REG, 14);
    port_byte_out(VGA_DATA_REG, (unsigned char)(offset >> 8));
    port_byte_out(VGA_CTRL_REG, 15);
    port_byte_out(VGA_DATA_REG, (unsigned char)(offset & 0xff));
}

void printChar(char c, int offset, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    if (offset >= 2 * MAX_COLS * MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
    }

    if (offset < 0) {
        offset = getCursorOffset();
    }

    if (c == '\n') {
        int row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++) 
            memory_copy((char *)get_offset(0, i) + VIDEO_ADDRESS,
                        (char *)get_offset(0, i-1) + VIDEO_ADDRESS,
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = (char *)get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
        for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    setCursorOffset(offset);
}



void VGAPrintPos(char *message, int col, int row) {
    int offset;

    if (col < 0 || row < 0) {
        offset = getCursorOffset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    } else {
        offset = get_offset(col, row);
    }

    int i = 0;
    while (message[i] != 0) {
        printChar(message[i++], get_offset(col, row), WHITE_ON_BLACK);
        offset = getCursorOffset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void VGAPrint(char *message) {
    VGAPrintPos(message, -1, -1);
}

void clearScreen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = (char *)VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    setCursorOffset(0);
}
