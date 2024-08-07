#include "util.h"

void intToAscii(int number, char str[]) {
    int index = 0;

    do {
        str[index++] = (number % 10) + '0';
        number /= 10;
    } while (number > 0);

    str[index] = '\0';

    // Reverse the string
    int start = 0;
    int end = index - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void memoryCopy(const void* src, void* dest, unsigned int nbytes)
{
    const uint8_t* s = src;
    uint8_t* d = dest;

    for (unsigned int i = 0; i < nbytes; i++)
    {
        d[i] = s[i];
    }
}
