#pragma once
#include "Peripheral.h"
#include "Messanger.h"

class Controller : public Peripheral, public Messanger {
	void pollAnalog();
	void pollDigital();

public:
	Controller(Processor*);

	void interrupt(uint16_t) override;
};

