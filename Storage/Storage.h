#pragma once
#include "../Hardware/Peripheral.h"
#include <fstream>
#include <string>

class Storage : public Peripheral {
	std::fstream file;

protected:
	enum class Error {
		UNKNOWN,
		CRITICAL,
		OUT_OF_BOUNDS,
		PROTECTED
	};

	uint8_t surfaces = 0;
	uint8_t tracks = 0;
	uint8_t sectors = 0;

	Storage(Processor*, Vendors, HWType, uint16_t, std::string, std::string);
	~Storage();

	void read(uint16_t, uint16_t);
	void write(uint16_t, uint16_t);

public:
	bool write_protect = false;
};
