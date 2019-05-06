#pragma once
#include "Wireless.h"

class Transmitter : public Wireless {
	uint8_t power = 7;

protected:
	void setPower(uint8_t);
	uint8_t getPower();

	void transmit(Packet);

public:
	Transmitter(Processor*, Vendors, HWSubclass, HWAPI, uint16_t, uint16_t);
};

