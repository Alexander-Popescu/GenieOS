#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../util/intdef.h"

void initKeyboard();
char *scancodeToAscii(uint8_t scancode);
char *scancodeToLetter(uint8_t scancode);

typedef struct {
	bool shift;
	bool alt;
	bool ctrl;
	char inputs[256];
	uint8_t index;
} KB_STATE;

#endif
