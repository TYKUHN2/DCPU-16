#pragma once
#include "../Hardware/Peripheral.h"

struct Packet {
	uint16_t* data;
	uint8_t size;
};

class Transmitter;

class Wireless : public Peripheral
{
	friend Transmitter;

protected:
	uint8_t channel = 0;

	virtual void receive(Packet) = 0;

public:
	Wireless(Processor*, Vendors, HWSubclass, HWAPI, uint16_t, uint16_t);
};

