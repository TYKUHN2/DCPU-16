#include "Hardware.h"

Hardware::Hardware(Processor * target, uint32_t manufacturer, uint32_t type, uint16_t version) : parent(target), type(type), manufacturer(manufacturer), version(version)
{
	target->connect(this);
}
