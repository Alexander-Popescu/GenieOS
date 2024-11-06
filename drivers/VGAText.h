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

//misc
#define OFFSET_LIMIT MAX_COLS * MAX_ROWS - 1

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
#endif
