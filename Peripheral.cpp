#include "Peripheral.h"

Peripheral::Peripheral(Processor * target, Vendors manufacturer, HWType type, uint16_t version) : parent(target), type(type.raw), manufacturer((uint32_t)manufacturer), version(version)
{
	target->connect(this);
}
