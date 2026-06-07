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

  printString("\nGenie> ", -1, VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN));
}

// ── splash screen ──────────────────────────────────────────────────

#define SPLASH_TOP    1
#define SPLASH_LEFT   2
#define SPLASH_WIDTH  72

static void stamp(int row, int col, char *str, uint8_t attr) {
  printString(str, row * MAX_COLS + col, attr);
}

static void drawSplashFrame(int contentRows) {
  uint8_t f = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
  int T = SPLASH_TOP;
  int L = SPLASH_LEFT;
  int R = L + SPLASH_WIDTH - 1;

  printChar('\xDA', T * MAX_COLS + L, f);
  for (int i = 1; i < SPLASH_WIDTH - 1; i++)
    printChar('\xC4', T * MAX_COLS + L + i, f);
  printChar('\xBF', T * MAX_COLS + R, f);

  for (int r = 1; r <= contentRows; r++) {
    printChar('\xB3', (T + r) * MAX_COLS + L, f);
    printChar('\xB3', (T + r) * MAX_COLS + R, f);
  }

  printChar('\xC0', (T + contentRows + 1) * MAX_COLS + L, f);
  for (int i = 1; i < SPLASH_WIDTH - 1; i++)
    printChar('\xC4', (T + contentRows + 1) * MAX_COLS + L + i, f);
  printChar('\xD9', (T + contentRows + 1) * MAX_COLS + R, f);
}

#define SPLASH_ROWS   11
#define WISH_ROW      8
#define WISH_COL      43

static void drawSplashArt() {
  uint8_t S = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
  uint8_t L = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_YELLOW);
  uint8_t T = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
  uint8_t B = VGA_ATTR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);
  uint8_t W = getWishColor();

  //        col:  4    8   12   16   20   24   28   32   36   40
  //               |    |    |    |    |    |    |    |    |    |
  //
  //  handle (left side)
  /*  5 */  stamp( 7,  8,  "\xDB\xDF\xDF\xDF",                               L);
  /*  6 */  stamp( 8,  9,  "\xDF\xDF\xDF\xDC",                               L);
  //
  //  lid + neck
  /*  5 */  stamp( 7, 16,  "\xDC\xDB\xDB\xDB\xDB\xDC",                   L);
  //
  //  body (flat top, no shoulders)
  /*  6 */  stamp( 8, 14,  "\xDC\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDC",  L);
  /*  7 */  stamp( 9, 13,  "\xDE\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDF", L);
  //
  //
  //  body bottom + stand
  /*  8 */  stamp( 10, 14,  "\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF",      L);
  /*  9 */  stamp( 10, 16,  "\xDB\xDB\xDB\xDB\xDB\xDB\xDB",                   L);
  //
  //  base
  /* 11 */  stamp(11, 13,  "\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF", L);
  //
  //  smoke (rises from spout, gap then wavy upward)
  /*  2 */  stamp( 3, 32,  "\xB0\xB0\xB0\xB0",                               S);
  /*  2 */  stamp( 3, 44,  "Welcome to GenieOS",                                   T);
  /*  3 */  stamp( 4, 30,  "\xB0\xB1\xB1\xB1\xB0",                           S);
  /*  4 */  stamp( 5, 29,  "\xB0\xB1\xB2\xB2\xB1\xB0",                       S);
  /*  5 */  stamp( 6, 28,  "\xB1\xB2\xB2\xB1",                               S);
  /*  5 */  stamp( 5, 45,  "LampShell v0.1",                           B);
  /*  6 */  stamp( 7, 28,  "\xB1\xB2\xB1",                                   S);
  /*  7 */  stamp( 8, 28,  "\xB2",                                           S);

  // wish counter on base row
  stamp(WISH_ROW, WISH_COL, "Remaining Wishes: ", W);
  char wishStr[4];
  intToAscii(shell_state.wishes, wishStr);
  stamp(WISH_ROW, WISH_COL + 18, wishStr, W);
}

static void drawSplash() {
  clearScreen();
  setCursorLocation(0);
  drawSplashFrame(SPLASH_ROWS);
  drawSplashArt();
  shell_state.splashVisible = true;
  setCursorLocation((SPLASH_TOP + SPLASH_ROWS + 2) * MAX_COLS);
}

void shellInit() {
  shell_state.wishes = 3;
  drawSplash();
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
  shell_state.splashVisible = false;
}

static void cmd_splash() {
  drawSplash();
}

static void handleShellCommand(char *command) {
  if (shell_state.inputIndex == 0) {
      newCmdLine();
      return;
  }
  
  bool found = false;
  bool isSplash = false;
  bool isClear = false;
  for (int i = 0; i < NUM_COMMANDS; i++) {
      ShellCommand cmd = get_command(i);
      if (compareString(command, cmd.name)) {
          isSplash = compareString(cmd.name, "splash");
          isClear = compareString(cmd.name, "clear");
          if (shell_state.wishes > 0) shell_state.wishes--;
          cmd.execute();
          found = true;
          break;
      }
  }

  if (found) {
    if (!isSplash) {
      if (shell_state.splashVisible) {
        uint16_t savedCursor = getCursorOffset();
        stamp(WISH_ROW, WISH_COL, "Remaining Wishes: ", getWishColor());
        char ws[4];
        intToAscii(shell_state.wishes, ws);
        stamp(WISH_ROW, WISH_COL + 18, ws, getWishColor());
        stamp(WISH_ROW, WISH_COL + 19, " ", getWishColor());
        setCursorLocation(savedCursor);
      } else {
        char wishStr[4];
        intToAscii(shell_state.wishes, wishStr);
        printString("\nRemaining Wishes: ", -1, getWishColor());
        printString(wishStr, -1, getWishColor());
      }
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
