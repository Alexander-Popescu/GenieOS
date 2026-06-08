#include "timer.h"
#include "../drivers/VGAText.h"
#include "../util/util.h"
#include "isr.h"

#define CLOCK_RATE 1193182
#define PIT_FREQUENCY 100

static volatile uint32_t tick = 0;

static void timer_callback(registers_t regs) {
	tick++;
}

void init_timer(uint32_t frequency) {
	// add timer callback as IRQ 0
	addInterruptHandler(IRQ0, timer_callback);

	//get PIT divisor value, hardware clock runs at 1193182 Hz
	uint32_t divisor = CLOCK_RATE / frequency;
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	//send command
	wIO8(0x43, 0x36); // command port
	wIO8(0x40, low);
	wIO8(0x40, high);
}

uint32_t get_tick() {
	return tick;
}
