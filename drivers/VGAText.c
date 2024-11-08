#include "VGAText.h"

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
	offset = (rIO8(VGA_DATA_REG) << 8);

	//now low byte
	wIO8(VGA_CTRL_REG, CURSOR_LOW_BYTE);
	offset |= rIO8(VGA_DATA_REG);

	return offset;
}

void printError(uint8_t attribute) {
	//prints a red E in the bottom right corner to indicate error
	printChar('E', OFFSET_LIMIT, attribute);
}

uint16_t printChar(char c, int16_t offset, uint8_t attribute) {
	if (c == '\n') {
		//calculate new offset and set cursor
		uint16_t currentOffset = getCursorOffset();
		if (currentOffset > (OFFSET_LIMIT - MAX_COLS))
		{
			//last line edgecase
			scrollScreen();
			offset = OFFSET_LIMIT - MAX_COLS + 1;
			setCursorLocation(offset);
			return offset;

		} else {
			uint16_t offset = (currentOffset - (currentOffset % MAX_COLS)) + MAX_COLS;
			setCursorLocation(offset);
			return offset;
		}
	}
	
	if (offset > OFFSET_LIMIT ) {
		printError(RED_ON_WHITE);
		return OFFSET_LIMIT + 1;
	} else {
		if (offset < 0) {
			offset = getCursorOffset();
		}
		if (attribute == 0) {
			//we assume an invisible attribute indicates default, and that the preferred method of blank spacing is the space character;
			attribute = WHITE_ON_BLACK;
		}
		char* vRam = (char *)VRAM_START_ADDRESS;
		vRam[offset * 2] = c;
		vRam[offset * 2 + 1] = attribute;
	}

	offset += 1;
	setCursorLocation(offset);	
	return offset;
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

void scrollScreen() {
    uint8_t* src = (uint8_t*)(VRAM_START_ADDRESS) + MAX_COLS * 2;
    uint8_t* dest = (uint8_t*)(VRAM_START_ADDRESS);

    memoryCopy(src, dest, (MAX_COLS * (MAX_ROWS - 1)) * 2);


	for (int i = 0; i < MAX_COLS; i++) {
		printChar(' ', MAX_COLS * (MAX_ROWS - 1) + i, 0);
	}

	setCursorLocation(MAX_COLS * (MAX_ROWS - 1));

}

void printString(char* string, int16_t offset, uint8_t attribute) {
	if (offset < 0) {
		offset = getCursorOffset();
	}
	int i = 0;
	while (string[i] != 0) {
		offset = printChar(string[i++], offset, attribute);
		if (offset > OFFSET_LIMIT) {
			scrollScreen();
			offset = getCursorOffset();
		}
	}
}

static void backspace() {
    uint16_t offset = getCursorOffset();
    if (offset > 0) {
        printChar(' ', offset - 1, WHITE_ON_BLACK);
        setCursorLocation(offset - 1);
    }
}

void printBackspace(uint16_t count) {
    for (int i = 0; i < count; i++) {
        backspace();
    }
}
