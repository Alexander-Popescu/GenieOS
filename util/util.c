#include "util.h"

void memoryCopy(const void* src, void* dest, unsigned int nbytes)
{
    const uint8_t* s = src;
    uint8_t* d = dest;

    for (unsigned int i = 0; i < nbytes; i++)
    {
        d[i] = s[i];
    }
}
