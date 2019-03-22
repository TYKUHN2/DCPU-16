#pragma once
#include "Peripheral.h"
#include "Port.h"
#include <vector>

class Wired : public Peripheral {
protected:
	Port port{ this };

public:
	Wired(Processor*, Vendors, HWSubclass, HWAPI, uint16_t, uint16_t);

	virtual void receive(uint32_t, Port*) = 0;
	virtual void onConnect(Port*) = 0;
};

