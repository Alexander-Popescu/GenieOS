#include "timer.h"
#include "../drivers/VGAText.h"
#include "../util/util.h"
#include "isr.h"

// hardware oscillator frequency
#define clockRate 1192182

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
	tick++;
	printString("Tick: ", -1, 0);

	char tick_ascii[256];
	intToAscii(tick, tick_ascii);
	printString(tick_ascii, -1, 0);
	printString("\n", -1, 0);
}

void init_timer(uint32_t frequency) {
	// add timer callback as IRQ 0
	addInterruptHandler(IRQ0, timer_callback);

	//get PUT value hardware clock at 1193180
	uint32_t divisor = clockRate / frequency;
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	//send command
	wIO8(0x43, 0x36); // command port
	wIO8(0x40, low);
	wIO8(0x40, high);
}
