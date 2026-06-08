#ifndef SHELL_H
#define SHELL_H

#include "../drivers/keyboard.h"
#include "../drivers/VGAText.h"

#define inputBufferSize 256
#define MAX_COMMANDS 32

extern KB_STATE kb_state;

typedef struct {
	char inputBuffer[inputBufferSize];
	uint8_t inputIndex;
	int8_t wishes;
	bool splashVisible;
	uint32_t splashScrollOrigin;
	bool inGraphicsMode;
} SHELL_STATE;

// Command function pointer
typedef void (*CommandFunc)(void);

typedef struct {
    char *name;
    char *description;
    CommandFunc execute;
} ShellCommand;

void register_command(char *name, char *description, CommandFunc execute);
void shellUpdate();
void shellInit();

#endif
