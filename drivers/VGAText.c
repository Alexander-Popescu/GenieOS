#include "VGAText.h"
#include "IOports.h"

int clearScreen() {
	char *vRam = (char *)VRAM_START_ADDRESS;

	for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
		vRam[i*2] = ' '; // blank character
	}

	return 0;
}

void setCursorLocation(uint16_t offset) {
	//offset is in cells
	uint16_t location = offset;

	//set high byte
	wIO8(VGA_CTRL_REG, CURSOR_HIGH_BYTE);
	wIO8(VGA_DATA_REG, (uint8_t)(location >> 8));

	//now low byte
	wIO8(VGA_CTRL_REG, CURSOR_LOW_BYTE);
	wIO8(VGA_DATA_REG, (uint8_t)(location & 0xFF));
}

void initScreen() {
	clearScreen();
	setCursorLocation(0);
}

uint16_t getCursorOffset() {
	uint16_t offset;

	//get high byte
	wIO8(VGA_CTRL_REG, CURSOR_HIGH_BYTE);
	offset &= (rIO8(VGA_DATA_REG) << 8);

	//now low byte
	wIO8(VGA_CTRL_REG, CURSOR_LOW_BYTE);
	offset &= rIO8(VGA_DATA_REG);

	return offset;
}

void printError(uint8_t attribute) {
	//prints a red E in the bottom right corner to indicate error
	printChar('E', OFFSET_LIMIT, attribute);
}

void printChar(char c, uint16_t offset, uint8_t attribute) {
	

	if (offset > OFFSET_LIMIT ) {
		printError(RED_ON_WHITE);
		return;
	} else {
		if (attribute == 0) {
			//we assume an invisible attribute indicates default, and that the preferred method of blank spacing is the space character;
			attribute = WHITE_ON_BLACK;
		}
		char* vRam = (char *)VRAM_START_ADDRESS;
		vRam[offset * 2] = c;
		vRam[offset * 2 + 1] = attribute;
	}

	setCursorLocation(offset + 1);	
	return;
}

void setAttrib(uint8_t attribute, uint16_t offset) {
	if (offset > OFFSET_LIMIT ) {
		printError(BLUE_ON_WHITE);
		return;
	} else {
		char* vRam = (char *)VRAM_START_ADDRESS;
		vRam[offset * 2 + 1] = attribute;
	}

	return;
}
