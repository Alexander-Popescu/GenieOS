#ifndef VGATEXT_H
#define VGATEXT_H

#define VRAM_START_ADDRESS 0xB8000
#define MAX_COLS 80
#define MAX_ROWS 25

//vga ports
#define VGA_CTRL_REG 0x3D4
#define VGA_DATA_REG 0x3D5

//vga "commands"
#define CURSOR_HIGH_BYTE 0x0E
#define CURSOR_LOW_BYTE 0x0F

//default attributes
#define WHITE_ON_BLACK 0x0F
#define RED_ON_WHITE 0xF4
#define BLUE_ON_WHITE 0x71

// VGA Colors
#define VGA_COLOR_BLACK 0x00
#define VGA_COLOR_BLUE 0x01
#define VGA_COLOR_GREEN 0x02
#define VGA_COLOR_CYAN 0x03
#define VGA_COLOR_RED 0x04
#define VGA_COLOR_MAGENTA 0x05
#define VGA_COLOR_BROWN 0x06
#define VGA_COLOR_LIGHT_GRAY 0x07
#define VGA_COLOR_DARK_GRAY 0x08
#define VGA_COLOR_LIGHT_BLUE 0x09
#define VGA_COLOR_LIGHT_GREEN 0x0A
#define VGA_COLOR_LIGHT_CYAN 0x0B
#define VGA_COLOR_LIGHT_RED 0x0C
#define VGA_COLOR_LIGHT_MAGENTA 0x0D
#define VGA_COLOR_YELLOW 0x0E
#define VGA_COLOR_WHITE 0x0F

// Attribute generator (bg << 4 | fg)
#define VGA_ATTR(bg, fg) ((bg << 4) | fg)

//misc
#define OFFSET_LIMIT MAX_COLS * MAX_ROWS - 1
#define SHELL_MARGIN 2

#include "../util/intdef.h"
#include "../drivers/IOports.h"
#include "../util/util.h"

int clearScreen();
void setCursorLocation(uint16_t offset);
uint16_t getCursorOffset();

void initScreen();

uint16_t printChar(char c, int16_t offset, uint8_t attribute);
void setAttrib(uint8_t attribute, uint16_t offset);

void printString(char* string, int16_t offset, uint8_t attribute);
void printBackspace(uint16_t count);

void scrollScreen();

extern uint32_t screenScrollCount;

#endif
