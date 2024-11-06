#include "keyboard.h"
#include "IOports.h"
#include "../cpu/isr.h"
#include "../apps/shell.h"

KB_STATE kb_state;

static void keyboard_callback(registers_t regs) {
	//keyboard scancode gets left in port 0x60
	uint8_t scancode = rIO8(0x60);
        kb_state.index = kb_state.index++ % 256;
        kb_state.inputs[kb_state.index] = scancode;

        // Set special key states based on scancode
        kb_state.shift = (scancode == 0x2A || scancode == 0x36) ? 1 : (scancode == 0xAA || scancode == 0xB6) ? 0 : kb_state.shift;
        kb_state.ctrl = (scancode == 0x1D) ? 1 : (scancode == 0x9D) ? 0 : kb_state.ctrl;
        kb_state.alt = (scancode == 0x38) ? 1 : (scancode == 0xB8) ? 0 : kb_state.alt;

        shellUpdate(scancode);
        return;
}

void initKeyboard() {
	//link scancode handler to the interrupt from PIC
	addInterruptHandler(IRQ1, keyboard_callback);
        
        return;
}

//translate scancode to ascii value, basically lookup tables

char *scancodeToAscii(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            return "ERROR";
        case 0x1:
            return "ESC";
        case 0x2:
            return "1";
        case 0x3:
            return "2";
        case 0x4:
            return "3";
        case 0x5:
            return "4";
        case 0x6:
            return "5";
        case 0x7:
            return "6";
        case 0x8:
            return "7";
        case 0x9:
            return "8";
        case 0x0A:
            return "9";
        case 0x0B:
            return "0";
        case 0x0C:
            return "-";
        case 0x0D:
            return "+";
        case 0x0E:
            return "Backspace";
        case 0x0F:
            return "Tab";
        case 0x10:
            return "Q";
        case 0x11:
            return "W";
        case 0x12:
            return "E";
        case 0x13:
            return "R";
        case 0x14:
            return "T";
        case 0x15:
            return "Y";
        case 0x16:
            return "U";
        case 0x17:
            return "I";
        case 0x18:
            return "O";
        case 0x19:
            return "P";
        case 0x1A:
            return "[";
        case 0x1B:
            return "]";
        case 0x1C:
            return "ENTER";
        case 0x1D:
            return "LCtrl";
        case 0x1E:
            return "A";
        case 0x1F:
            return "S";
        case 0x20:
            return "D";
        case 0x21:
            return "F";
        case 0x22:
            return "G";
        case 0x23:
            return "H";
        case 0x24:
            return "J";
        case 0x25:
            return "K";
        case 0x26:
            return "L";
        case 0x27:
            return ";";
        case 0x28:
            return "'";
        case 0x29:
            return "`";
        case 0x2A:
            return "LShift";
        case 0x2B:
            return "\\";
        case 0x2C:
            return "Z";
        case 0x2D:
            return "X";
        case 0x2E:
            return "C";
        case 0x2F:
            return "V";
        case 0x30:
            return "B";
        case 0x31:
            return "N";
        case 0x32:
            return "M";
        case 0x33:
            return ",";
        case 0x34:
            return ".";
        case 0x35:
            return "/";
        case 0x36:
            return "Rshift";
        case 0x37:
            return "Keypad *";
        case 0x38:
            return "LAlt";
        case 0x39:
            return "Spc";
        default:
            if (scancode <= 0x7f) {
                return "Unknown key down";
            } else if (scancode <= 0x39 + 0x80) {
                return "Key up";
            } else {
                return "Unknown key up";
            }
    }
}

char *scancodeToLetter(uint8_t scancode) {
    switch (scancode) {
        case 0x2:
            return "1";
        case 0x3:
            return "2";
        case 0x4:
            return "3";
        case 0x5:
            return "4";
        case 0x6:
            return "5";
        case 0x7:
            return "6";
        case 0x8:
            return "7";
        case 0x9:
            return "8";
        case 0x0A:
            return "9";
        case 0x0B:
            return "0";
        case 0x0C:
            return "-";
        case 0x0D:
            return "+";
        case 0x10:
            return "Q";
        case 0x11:
            return "W";
        case 0x12:
            return "E";
        case 0x13:
            return "R";
        case 0x14:
            return "T";
        case 0x15:
            return "Y";
        case 0x16:
            return "U";
        case 0x17:
            return "I";
        case 0x18:
            return "O";
        case 0x19:
            return "P";
        case 0x1A:
            return "[";
        case 0x1B:
            return "]";
        case 0x1E:
            return "A";
        case 0x1F:
            return "S";
        case 0x20:
            return "D";
        case 0x21:
            return "F";
        case 0x22:
            return "G";
        case 0x23:
            return "H";
        case 0x24:
            return "J";
        case 0x25:
            return "K";
        case 0x26:
            return "L";
        case 0x27:
            return ";";
        case 0x28:
            return "'";
        case 0x29:
            return "`";
        case 0x2B:
            return "\\";
        case 0x2C:
            return "Z";
        case 0x2D:
            return "X";
        case 0x2E:
            return "C";
        case 0x2F:
            return "V";
        case 0x30:
            return "B";
        case 0x31:
            return "N";
        case 0x32:
            return "M";
        case 0x33:
            return ",";
        case 0x34:
            return ".";
        case 0x35:
            return "/";
        case 0x39:
            return " ";
        default:
            return "";
    }
}
