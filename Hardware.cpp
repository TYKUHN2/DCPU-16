#include "Hardware.h"

Hardware::Hardware(Processor * target, HardwareType type) : parent(target), type(type)
{
	target->connect(this);
}
