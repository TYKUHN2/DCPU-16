#include "Speaker.h"
#include "../config.h"

constexpr HWClass speaker {
	HWClass::GENERIC_OUTPUT,
	0
};

constexpr uint16_t uniqueid = 0x0001;
constexpr uint16_t vers		= 0x0001;

constexpr HWAPI speakerapi{
	0xF,	//No standard API
	false,	//No extended API
	false,	//Not memory mapped
	false,	//No reset command
	false	//Does not generate interrupts
};

constexpr HWType speakertype {
	speaker,
	speakerapi,
	uniqueid
};

Speaker::Speaker(Processor * proc) : Peripheral(proc, Vendors::VARTOK_HARDWARE, speakertype, vers) {
	chans = new DEFAULT_WAVE[2];
	chans[1].setVolume(0.9f);
}

void Speaker::interrupt(uint16_t command) {
	uint16_t freq = parent->registers.b;

	if (command < 0x0002)
		chans[command].setFrequency(freq);
}
