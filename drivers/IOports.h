#ifndef PORTS_H
#define PORTS_H
#include "../util/intdef.h"

void wIO8(uint16_t port, uint8_t data);
uint8_t rIO8(uint16_t port);

void wIO16(uint16_t port, uint16_t data);
uint16_t rIO16(uint16_t port);


#endif
