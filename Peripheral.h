#pragma once
#include "Processor.h"
#include "HWType.h"
#include "Vendors.h"
#include <cstdint>



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

