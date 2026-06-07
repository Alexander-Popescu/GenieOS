#include "shell.h"

SHELL_STATE shell_state;

static uint8_t getWishColor() {
  switch (shell_state.wishes) {
    case 3:  return VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
    case 2:  return VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_YELLOW);
    default: return VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_RED);
  }
}

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
  while (shell_state.inputIndex > 0) {
      shell_state.inputIndex--;
      shell_state.inputBuffer[shell_state.inputIndex] = '\0';
  }
  shell_state.inputBuffer[0] = '\0';

  printString("\n  Genie> ", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN));
}

static void printSplash() {
  clearScreen();
  setCursorLocation(0);

  uint8_t frameColor = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
  uint8_t titleColor = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
  uint8_t artColor1 = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_YELLOW);
  uint8_t artColor2 = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);

  printString("\n  \xDA", -1, frameColor);
  for (int i = 0; i < 70; i++) printChar('\xC4', -1, frameColor);
  printString("\xBF\n", -1, frameColor);
  printString("  \xB3                                                                      \xB3\n", -1, frameColor);
  printString("  \xB3             ", -1, frameColor);
  printString("\xB0", -1, artColor2);
  printString("                                                        \xB3\n", -1, frameColor);
  printString("  \xB3            ", -1, frameColor);
  printString("\xB1\xB1\xB1", -1, artColor2);
  printString("                    ", -1, frameColor);
  printString("GenieOS v0.1", -1, titleColor);
  printString("                       \xB3\n", -1, frameColor);
  printString("  \xB3            ", -1, frameColor);
  printString("\xB2\xB2\xB2\xB2\xB2", -1, artColor2);
  printString("                                                     \xB3\n", -1, frameColor);
  printString("  \xB3        ", -1, frameColor);
  printString("\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC", -1, artColor1);
  printString("           LampShell Interactive                   \xB3\n", -1, frameColor);
  printString("  \xB3         ", -1, frameColor);
  printString("\xDE\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDD", -1, artColor1);
  printString("                                                \xB3\n", -1, frameColor);
  printString("  \xB3        ", -1, frameColor);
  printString("\xDF\xDF\xDF\xDF\xDB\xDB\xDF\xDF\xDF", -1, artColor1);
  printString("                                                     \xB3\n", -1, frameColor);
  printString("  \xB3       ", -1, frameColor);
  printString("\xDC\xDC\xDC\xDC\xDC\xDB\xDB\xDC\xDC\xDC\xDC\xDC", -1, artColor1);
  char wishStr[4];
  intToAscii(shell_state.wishes, wishStr);
  printString("             Remaining Wishes: ", -1, getWishColor());
  printString(wishStr, -1, getWishColor());
  printString("                   \xB3\n", -1, frameColor);
  printString("  \xB3         ", -1, frameColor);
  printString("\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF", -1, artColor1);
  printString("                                                \xB3\n", -1, frameColor);

  printString("  \xB3                                                                      \xB3\n", -1, frameColor);

  printString("  \xC0", -1, frameColor);
  for (int i = 0; i < 70; i++) printChar('\xC4', -1, frameColor);
  printString("\xD9\n\n", -1, frameColor);
}

void shellInit() {
  shell_state.wishes = 3;
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

static void cmd_help();
static void cmd_clear();
static void cmd_splash();

#define NUM_COMMANDS 3

static ShellCommand get_command(int index) {
    ShellCommand cmd;
    switch (index) {
        case 0:
            cmd.name = "help";
            cmd.description = "Show this help message";
            cmd.execute = cmd_help;
            break;
        case 1:
            cmd.name = "clear";
            cmd.description = "Clear the terminal screen";
            cmd.execute = cmd_clear;
            break;
        case 2:
            cmd.name = "splash";
            cmd.description = "Display the GenieOS splash art";
            cmd.execute = cmd_splash;
            break;
    }
    return cmd;
}

static void cmd_help() {
  printString("\nLampShell built-in commands:\n", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GRAY));
  for (int i = 0; i < NUM_COMMANDS; i++) {
    ShellCommand cmd = get_command(i);
    printString("  ", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    printString(cmd.name, -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    int padding = 10 - strlength(cmd.name);
    for (int p = 0; p < padding; p++) {
        printString(" ", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    }
    printString("- ", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    printString(cmd.description, -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
    printString("\n", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN));
  }
}

static void cmd_clear() {
  clearScreen();
  setCursorLocation(0);
}

static void cmd_splash() {
  printSplash();
}

static void handleShellCommand(char *command) {
  if (shell_state.inputIndex == 0) {
      newCmdLine();
      return;
  }
  
  bool found = false;
  bool isSplash = false;
  for (int i = 0; i < NUM_COMMANDS; i++) {
      ShellCommand cmd = get_command(i);
      if (compareString(command, cmd.name)) {
          isSplash = compareString(cmd.name, "splash");
          if (shell_state.wishes > 0) shell_state.wishes--;
          cmd.execute();
          found = true;
          break;
      }
  }

  if (found) {
    if (!isSplash) {
      char wishStr[4];
      intToAscii(shell_state.wishes, wishStr);
      printString("\nRemaining Wishes: ", -1, getWishColor());
      printString(wishStr, -1, getWishColor());
    }
  } else {
    printString("\nCommand '", -1, 0);
    printString(command, -1, 0);
    printString("' not found.", -1, 0);
  }
  
  newCmdLine();
}

void shellUpdate() {
  uint8_t scancode = kb_state.inputs[kb_state.index];

  // Ignore key release events
  if (scancode & 0x80) return;

  if (scancode == 0x0E) {
    shellBackspace(1, true);
    return;
  }
  if (scancode == 0x1C) {
    handleShellCommand(shell_state.inputBuffer);
    return;
  }

  handleCharInput(scancode);
}
