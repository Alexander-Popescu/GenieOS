#ifndef IDT_H
#define IDT_H

#include "../util/intdef.h"

//table of 255 entries, and a pointer structure for the register

typedef struct {
	uint16_t addressLow; //low address of function start
	uint16_t segSelector; //segment selector
	uint8_t zero; // must be zero
	uint8_t flags;
	uint16_t addressHigh; //other half
} __attribute__((packed)) IDTEntry;

typedef struct {
	uint16_t limit;
	uint32_t base; //range of idt table
} __attribute__((packed)) IDTRegister;

void fillIDTEntry(int interrupt, uint32_t handler);
void initIDT();

#endif
