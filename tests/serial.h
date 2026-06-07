#ifndef SERIAL_H
#define SERIAL_H

#include "../util/intdef.h"

#define SERIAL_COM1 0x3F8

void initSerial();
void serialWriteChar(char c);
void serialWriteString(char *str);
void serialWriteInt(int n);

#endif
