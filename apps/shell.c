#include "shell.h"

//track input buffer and location on VGA
SHELL_STATE shell_state;

static void shellBackspace(uint8_t count, bool erase) {
    for (int i = 0; i < count; i++) {
        shell_state.inputBuffer[shell_state.inputIndex] = 0;
        if (shell_state.inputIndex != 0) {
            shell_state.inputIndex--;
            shell_state.inputBuffer[shell_state.inputIndex] = '\0';
            if (erase) {
                printString(" ", getCursorOffset() - 1, 0);
                setCursorLocation(getCursorOffset() - 1);
            }
        }
    }
}

static void newCmdLine() {
    shellBackspace(shell_state.inputIndex, false);
    printString("\n>>", -1, 0);
}

void shellInit() {
    printString("Welcome to GenieOS!\n", -1, 0x7);
    printString("Using LampShell v0.1", -1, 0x7);
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
        const char *original = "1234567890-+[]\\;',./`";
        const char *alternate = "!@#$%^&*()_={}|:\"<>?~";

        // search original string for corrosponding alternate
        for (int i = 0; original[i] != '\0'; i++) {
            if (*chr == original[i]) {
                *chr = alternate[i];
                return;
            }
        }
    }
   
}

static void shellInput(char chr) {
    shell_state.inputBuffer[shell_state.inputIndex] = chr;
    shell_state.inputIndex++;
    shell_state.inputBuffer[shell_state.inputIndex] = '\0';
}

static void handleCharInput(uint8_t scancode) {
    // Get letter scancode and set casing/effects correctly
    char *ascii = scancodeToLetter(kb_state.inputs[kb_state.index]);
    char letter[2] = { *ascii, '\0' };
    kb_state.shift ? handleShift(letter) : chrToLower(letter);

    if (letter[0] != '\0' && shell_state.inputIndex != (uint8_t)(inputBufferSize - 1)) {
        shellInput(letter[0]);
        printString(&letter[0], -1, 0);
    }

}

static void handleShellCommand(char *command) {
    //clean input, TODO
    if (compareString(command, "clear")) {
        //probably can be made better,but starts a new CmdLine on the first row
        clearScreen();
        shellBackspace(shell_state.inputIndex, false);
        setCursorLocation(0);
        printString(">>", -1, 0);
    } else {
        if (*command != '\0') { //exception: empty input buffer
            //pass in the input if the command wasnt found, default branch
            printString("\nCommand '", -1, 0);
            printString(command, -1, 0);
            printString("' not found.", -1, 0);
        }
        newCmdLine();
    }
}

void shellUpdate() {
    //pull last input from keyboard buffer, assume update was called on keypress
    uint8_t scancode = kb_state.inputs[kb_state.index];

    //check backspace keycode
    if (scancode == 0x0E) {
        //also updates VGAText buffer to reflect backspace
	shellBackspace(1, true);
	return;
    }
    // enter key for testing
    if (scancode == 0x1C) {
        handleShellCommand(shell_state.inputBuffer);
        return;
    }
    
    //also prints inputs to VGAText buffer
    handleCharInput(scancode);

    return;
}
