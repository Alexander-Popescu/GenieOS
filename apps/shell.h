#ifndef SHELL_H
#define SHELL_H

#include "../drivers/keyboard.h"
#include "../drivers/VGAText.h"
extern KB_STATE kb_state;

void shellUpdate(uint8_t scancode);

void shellInit();

#endif
