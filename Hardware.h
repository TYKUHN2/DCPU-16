#pragma once
#include "Interruptable.h"
#include "HardwareType.h"
#include "Processor.h"

class Hardware : public Interruptable
{
	HardwareType type;
	Processor * parent;

	bool hasReceived = false;

public:
	Hardware(Processor *, HardwareType type);
};

