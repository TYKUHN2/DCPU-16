#include "Messagable.h"

void Messagable::interrupt(uint16_t msg) {
	if (interrupts == 256) {
		overflow();

		interrupts = 0;
		return;
	}

	interruptArr[interrupts++] = msg;
}

uint16_t Messagable::getInterrupt() {
	if (interrupts)
		return interruptArr[--interrupts];
	else
		throw -1;
}