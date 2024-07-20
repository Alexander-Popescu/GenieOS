#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

#define VGA_CTRL_REG 0x3d4
#define VGA_DATA_REG 0x3d5

#include "../kernel/util.h"

void clearScreen();
void VGAPrintPos(char *message, int col, int row);
void VGAPrint(char *message);
