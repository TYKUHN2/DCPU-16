#pragma once
#include <cstdint>

class Messagable {
	uint16_t interruptArr[256];

protected:
	int interrupts = 0;
public:
	virtual void interrupt(uint16_t);
	uint16_t getInterrupt();

	virtual void overflow() {};
};