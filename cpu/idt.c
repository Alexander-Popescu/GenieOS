#include "idt.h"

IDTEntry idt[256]; //actual table
IDTRegister idt_reg; //this will be thrown into the actual cpu register

void fillIDTEntry(int interrupt, uint32_t handler) {
	idt[interrupt].addressLow = (uint16_t)(handler & 0xFFFF);
	idt[interrupt].segSelector = 0x08;
	idt[interrupt].zero = 0;
	idt[interrupt].flags = 0x8E;
	idt[interrupt].addressHigh = (uint16_t)((handler >> 16) & 0xFFFF);
}

void initIDT() {
	idt_reg.base = (uint32_t)(&idt);	
	idt_reg.limit = 256 * sizeof(IDTEntry) - 1;

	//load register
	asm volatile ("lidtl (%0)": : "r" (&idt_reg));
}
