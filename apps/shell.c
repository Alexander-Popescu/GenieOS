#include "shell.h"

// track input buffer and location on VGA
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
  // Green prompt, light gray background, for a modern look
  printString("\nGenie> ", -1,
              VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN));
}

static void printSplash() {
  clearScreen();
  setCursorLocation(0);

  // Using Extended ASCII block characters for TUI frame and graphics
  // Frame color: Blue, Text: White, Art: Yellow/Cyan
  uint8_t frameColor = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
  uint8_t titleColor = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
  uint8_t artColor1 = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_YELLOW);
  uint8_t artColor2 = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);

  // CP437 Box drawing characters
  // \xDA = top left, \xBF = top right, \xC0 = bottom left, \xD9 = bottom right
  // \xC4 = horizontal line, \xB3 = vertical line

  // Total inner width is 70 characters.

  // Top border
  printString("\n  \xDA", -1, frameColor);
  for (int i = 0; i < 70; i++)
    printChar('\xC4', -1, frameColor);
  printString("\xBF\n", -1, frameColor);

  // Empty line (1 + 70 + 1)
  printString("  \xB3                                                          "
              "            \xB3\n",
              -1, frameColor);

  // Art line 1
  printString("  \xB3               ", -1, frameColor);
  printString("\xB0", -1, artColor2);
  printString("                                                      \xB3\n",
              -1, frameColor);

  // Art line 2
  printString("  \xB3              ", -1, frameColor);
  printString("\xB1\xB1\xB1", -1, artColor2);
  printString("                    ", -1, frameColor);
  printString("GenieOS v0.1", -1, titleColor);
  printString("                    \xB3\n", -1, frameColor);

  // Art line 3
  printString("  \xB3             ", -1, frameColor);
  printString("\xB2\xB2\xB2\xB2\xB2", -1, artColor2);
  printString("                                                   \xB3\n", -1,
              frameColor);

  // Art line 4
  printString("  \xB3          ", -1, frameColor);
  printString("\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC", -1, artColor1);
  printString("       LampShell Interactive           \xB3\n", -1, frameColor);

  // Art line 5
  printString("  \xB3         ", -1, frameColor);
  printString("\xDE\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDD", -1,
              artColor1);
  printString("                                              \xB3\n", -1,
              frameColor);

  // Art line 6
  printString("  \xB3          ", -1, frameColor);
  printString("\xDF\xDF\xDF\xDF\xDB\xDB\xDF\xDF\xDF", -1, artColor1);
  printString("                                                \xB3\n", -1,
              frameColor);

  // Art line 7
  printString("  \xB3         ", -1, frameColor);
  printString("\xDC\xDC\xDC\xDC\xDC\xDB\xDB\xDC\xDC\xDC\xDC\xDC", -1,
              artColor1);
  printString("       Ready to serve.                 \xB3\n", -1, frameColor);

  // Art line 8
  printString("  \xB3         ", -1, frameColor);
  printString("\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF", -1,
              artColor1);
  printString("                                              \xB3\n", -1,
              frameColor);

  // Empty line
  printString("  \xB3                                                          "
              "            \xB3\n",
              -1, frameColor);

  // Bottom border
  printString("  \xC0", -1, frameColor);
  for (int i = 0; i < 70; i++)
    printChar('\xC4', -1, frameColor);
  printString("\xD9\n\n", -1, frameColor);
}

void shellInit() {
  printSplash();
  newCmdLine();
}

static void handleShift(char *chr) {
  // if letter, we can set to uppercase and leave
  if (isAlpha(*chr)) {
    chrToUpper(chr);
    return;
  } else {
    // map keys with new shift values to the alternate
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
  char letter[2] = {*ascii, '\0'};
  kb_state.shift ? handleShift(letter) : chrToLower(letter);

  if (letter[0] != '\0' &&
      shell_state.inputIndex != (uint8_t)(inputBufferSize - 1)) {
    shellInput(letter[0]);
    printString(&letter[0], -1, 0);
  }
}

static void handleShellCommand(char *command) {
  // clean input, TODO
  if (compareString(command, "clear")) {
    // probably can be made better,but starts a new CmdLine on the first row
    clearScreen();
    shellBackspace(shell_state.inputIndex, false);
    setCursorLocation(0);
  } else if (compareString(command, "help")) {
    printString("\nLampShell built-in commands:\n", -1,
                VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GRAY));
    printString("  help    - Show this help message\n", -1,
                VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    printString("  clear   - Clear the terminal screen\n", -1,
                VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    printString("  splash  - Display the GenieOS splash art\n", -1,
                VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
  } else if (compareString(command, "splash")) {
    printSplash();
  } else {
    if (*command != '\0') { // exception: empty input buffer
      // pass in the input if the command wasnt found, default branch
      printString("\nCommand '", -1, 0);
      printString(command, -1, 0);
      printString("' not found.", -1, 0);
    }
  }
  newCmdLine();
}

void shellUpdate() {
  // pull last input from keyboard buffer, assume update was called on keypress
  uint8_t scancode = kb_state.inputs[kb_state.index];

  // check backspace keycode
  if (scancode == 0x0E) {
    // also updates VGAText buffer to reflect backspace
    shellBackspace(1, true);
    return;
  }
  // enter key for testing
  if (scancode == 0x1C) {
    handleShellCommand(shell_state.inputBuffer);
    return;
  }

  // also prints inputs to VGAText buffer
  handleCharInput(scancode);

  return;
}
