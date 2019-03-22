#pragma once
#include "Wireless.h"

class Timer {}; //Placeholder

class RACM : public Wireless, public Timer {
	uint16_t pin = 0x1234;
	bool safemode = false;

protected:
	void receive(Packet) override;

public:
	RACM(Processor*);

	void interrupt(uint16_t) override;
};

