#include "shell.h"
#include "../util/util.h"

static void newCmdLine() {
    printString("\n>>", -1, 0); 
}

void shellInit() {
    printString("Welcome to GenieOS!", -1, 0);
    newCmdLine();
}

static void handleShift(char *chr) {
    // if letter, we can set to uppercase and leave
    if (isAlpha(*chr)) {
	chrToUpper(chr);
	return;
    }
    else {
        //map keys with new shift values to the alternate
        const char *original = "1234567890-=[]\\;',./`";
        const char *alternate = "!@#$%^&*()_+{}|:\"<>?~";

        // search original string for corrosponding alternate
        for (int i = 0; original[i] != '\0'; i++) {
            if (*chr == original[i]) {
                *chr = alternate[i];
                return;
            }
        }
    }
   
}

static void handleCharInput(uint8_t scancode) {
    // Get letter scancode and set casing/effects correctly
    char *ascii = scancodeToLetter(kb_state.inputs[kb_state.index]);
    char letter[2] = { *ascii, '\0' };
    kb_state.shift ? handleShift(letter) : chrToLower(letter);
    printString(letter, -1, 0);
}


void shellUpdate(uint8_t scancode) {
    //check backspace keycode
    if (scancode == 0x0E) {
	printBackspace(1);
	return;
    }
    // enter key for testing
    if (scancode == 0x1C) {
        newCmdLine();
        return;
    }

    handleCharInput(scancode);

    return;
}
