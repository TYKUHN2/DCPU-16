#include "Wired.h"

Wired::Wired(Processor * proc, Vendors vendor, HWSubclass subclass, HWAPI api, uint16_t id, uint16_t vers) : Peripheral(proc, vendor, HWType{ HWClass{ HWClass::WIRED, (int)subclass }, api, id }, vers) {

}
