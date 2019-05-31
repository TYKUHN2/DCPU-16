#include "Peripheral.h"
#include "../Processor/Processor.h"
#include "../ROM/Byte_Order.h"

Peripheral::Peripheral(Processor * target, Vendors manufacturer, HWType type, uint16_t version) : parent(target), manufacturer((uint32_t)manufacturer), version(version)
{
	if (BYTE_ORDER == LITTLE_ENDIAN)
		type.hwclass = HWClass{ type.hwclass.subclass, type.hwclass.hwclass }; //Fix endianness

	uint8_t rawclass = *(uint8_t*)& type.hwclass;

	uint8_t rawapi = (type.api.id << 4) | (type.api.extended ? 0b1000 : 0b0000) | (type.api.memory ? 0b0100 : 0b0000) | (type.api.reset ? 0b0010 : 0b0000) | (type.api.interrupts ? 0b0001 : 0b0000);

	uint32_t raw = rawclass << 24 | rawapi << 16 | type.id;

	this->type = raw;

	target->connect(this);
}
