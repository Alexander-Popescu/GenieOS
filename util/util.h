#ifndef UTIL_H
#define UTIL_H

#include "intdef.h"

void intToAscii(int n, char str[]);

void memoryCopy(const void* src, void* dest, unsigned int nbytes);

void chrToLower(char *chr);
void chrToUpper(char *chr);
bool isNumeric(char chr);
bool isAlpha(char chr);

bool compareString(char *str1, char *str2);

uint16_t strlength(char *str);
#endif
