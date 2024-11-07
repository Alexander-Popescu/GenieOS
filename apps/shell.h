#ifndef SHELL_H
#define SHELL_H

#include "../drivers/keyboard.h"
#include "../drivers/VGAText.h"

#define inputBufferSize 256

extern KB_STATE kb_state;

typedef struct {
	//size of vga text mode window
	char inputBuffer[inputBufferSize];
	uint8_t inputIndex;
} SHELL_STATE;

void shellUpdate();

void shellInit();

#endif
