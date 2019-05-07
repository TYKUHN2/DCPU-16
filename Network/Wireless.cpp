#include "Wireless.h"
#include <vector>

std::vector<Wireless *> universe;

Wireless::Wireless(Processor * proc, Vendors vendor, HWSubclass subclass, HWAPI api, uint16_t id, uint16_t vers) : Peripheral(proc, vendor, HWType{ HWClass{HWClass::WIRELESS, (int)subclass}, api, id }, vers) {
	universe.push_back(this);
}
