#include "Transmitter.h"
#include <vector>

extern std::vector<Wireless*> universe;

Transmitter::Transmitter(Processor * proc, Vendors vendor, HWSubclass subclass, HWAPI api, uint16_t id, uint16_t vers) : Wireless(proc, vendor, subclass, api, id, vers) {
}

void Transmitter::setPower(uint8_t level) {
	if (level > 7)
		power = 7;
	else
		power = level;
}

inline uint8_t Transmitter::getPower() {
	return power;
}

void Transmitter::transmit(Packet packet) {

	for (int i = 0; i < universe.size(); i++) {
		Wireless * device = universe[i];

		if (device->channel == channel)
			device->receive(packet);
	}
}
