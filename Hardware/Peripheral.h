#pragma once
#include "HWInfo.h"
#include "Vendors.h"
#include "Messagable.h"
#include <cstdint>

class Processor;

class Peripheral : public Messagable
{
	bool hasReceived = false;

protected:
	Peripheral(Processor *, Vendors, HWType, uint16_t);

public:
	Processor * parent;

	uint32_t type;
	uint32_t manufacturer;
	uint16_t version;
};

