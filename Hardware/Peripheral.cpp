#include "Peripheral.h"
#include "../Processor/Processor.h"
#include "../ROM/Byte_Order.h"

Peripheral::Peripheral(Processor * target, Vendors manufacturer, HWType type, uint16_t version) : parent(target), manufacturer((uint32_t)manufacturer), version(version)
{
	if (BYTE_ORDER == LITTLE_ENDIAN)
		type.hwclass = HWClass{ type.hwclass.subclass, type.hwclass.hwclass }; //Fix endianness

	uint8_t rawclass = *(uint8_t*)& type.hwclass;
	uint16_t rawapi = *(uint16_t*)& type.api;

	uint32_t raw = rawclass << 24 | rawapi << 16 | type.id;

	swapBO((char*)&raw, 4, 1);

	this->type = raw;

	target->connect(this);
}
