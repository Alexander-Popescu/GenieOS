#include "serial.h"
#include "../drivers/IOports.h"

void initSerial() {
    wIO8(SERIAL_COM1 + 1, 0x00); // disable interrupts
    wIO8(SERIAL_COM1 + 3, 0x80); // enable DLAB (set baud divisor)
    wIO8(SERIAL_COM1 + 0, 0x03); // divisor lo: 38400 baud
    wIO8(SERIAL_COM1 + 1, 0x00); // divisor hi
    wIO8(SERIAL_COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    wIO8(SERIAL_COM1 + 2, 0xC7); // enable FIFO, clear, 14-byte threshold
    wIO8(SERIAL_COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static int serialTransmitEmpty() {
    return rIO8(SERIAL_COM1 + 5) & 0x20;
}

void serialWriteChar(char c) {
    while (!serialTransmitEmpty());
    wIO8(SERIAL_COM1, c);
}

void serialWriteString(char *str) {
    while (*str) {
        serialWriteChar(*str);
        str++;
    }
}

void serialWriteInt(int n) {
    char buf[12];
    int i = 0;

    if (n == 0) {
        serialWriteChar('0');
        return;
    }

    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        serialWriteChar(buf[j]);
    }
}
