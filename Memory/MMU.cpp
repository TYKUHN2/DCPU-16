#include "MMU.hpp"
#include "../Processor/Processor.h"

constexpr HWClass kcm{
	HWClass::MEMORY,
	(int)HWSubclass::MMU
};

constexpr uint16_t uniqueid = 0x110B;
constexpr uint16_t vers = 0x000B;

constexpr HWAPI kcmapi{
	0xE,	//Standard subclass API
	false,	//No extended API
	false,	//"Memory mapped"
	true,	//Reset command
	false	//Does not generate interrupts
};

constexpr HWType mmutype{
	kcm,
	kcmapi,
	uniqueid
};

enum class Commands {
	MAP_SEGMENT,
	MEMORY_INFO = 0x0004,
	RESET = 0xFFFF
};

MMU::MMU(Processor* parent, Executable base, uint16_t words) : Peripheral(parent, Vendors::KLINGSTON, mmutype, vers) {
	if (words < 15)
		words = 15;

	memory = new uint16_t[(words + 1) * 4096];
	size = words;

	resetMap();

	memcpy(memory, base.data, base.size);
}

uint16_t& MMU::operator[](uint16_t addr) {
	uint8_t segment = addr >> 12;

	PAGE page = ptable[segment];

	if (page.err) {
		bad = 0xFFFF;
		return bad;
	}

	uint32_t realaddr = (page.pword << 12) + (addr & 0x0FFF);

	return memory[realaddr];
}

void MMU::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::MAP_SEGMENT: {
		uint8_t vsegment = (uint8_t)parent->registers.b & 0xF;
		uint16_t psegment = parent->registers.c;

		if (psegment > size) {
			ptable[vsegment] = PAGE{ 0, true };
			parent->registers.a = 0;
		}
		else {
			ptable[vsegment] = PAGE{ psegment };
			parent->registers.a = 1;
		}
		break;
	}
	case Commands::MEMORY_INFO:
		parent->registers.b = size + 1;
		break;
	case Commands::RESET:
		resetMap();
	}
}

void MMU::resetMap() {
	for (uint16_t i = 0; i < 16; i++) {
		ptable[i] = PAGE{ i };
	}
}
