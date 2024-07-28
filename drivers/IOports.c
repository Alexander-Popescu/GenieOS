#include "IOports.h"

// "IN" instruction -> IN dst, src -> src is port
// "OUT" instruction -> OUT dst, src -> dst is port

void wIO8(uint16_t port, uint8_t data)
{
	asm volatile (
		"out %0, %1"
		:
		: "a" (data),
		  "d" (port)
	);
}

uint8_t rIO8(uint16_t port)
{
	uint8_t result;

	asm volatile (
		"in %1, %0"
		: "=a" (result)
		: "d" (port)
	);

	return result;
}

void wIO16(uint16_t port, uint16_t data)
{
	asm volatile (
			"out %0, %1"
			:
			: "a" (data),
			  "d" (port)
		);

}

uint16_t rIO16(uint16_t port)
{
	uint16_t result;

	asm volatile (
		"in %1, %0"
		: "=a" (result)
		: "d" (port)
	);

	return result;

}
