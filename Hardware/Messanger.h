#pragma once
#include "Messagable.h"

class Messanger
{
protected:
	uint16_t msg = 0;

	void message(Messagable *);
};

