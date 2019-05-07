#pragma once
#include "../Hardware/Peripheral.h"
#include "../ROM/Executable.h"

class MMU : public Peripheral {
	struct PAGE {
		uint16_t pword;
		bool err = false;
	};

	uint16_t* memory;
	uint16_t size;

	uint16_t bad = 0xFFFF;

	PAGE ptable[16];

	void resetMap();

public:
	MMU(Processor*, Executable, uint16_t);

	uint16_t& operator[](uint16_t);

	void interrupt(uint16_t) override;
};
