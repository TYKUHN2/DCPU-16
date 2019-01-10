#pragma once
#include "Interruptable.h"
#include "Processor.h"
#include <cstdint>

class Hardware : public Interruptable
{
	bool hasReceived = false;

	virtual void process() = 0;

	bool interruptHook(int) override;

public:
	Processor * parent;

	uint32_t type;
	uint32_t manufacturer;
	uint16_t version;

	Hardware(Processor *, uint32_t, uint32_t, uint16_t);
};

