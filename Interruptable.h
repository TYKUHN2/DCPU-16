#pragma once
#include "Ticking.h"

class Interruptable
{
	int * interruptQueue = new int[256];
	unsigned char interrupts = 0;

	virtual void overflow() {}; //Overflow event handler for interrupts. Default blank

	virtual bool interruptHook(int) { return true; }; //Hook on interrupts. Default returns true. True = Add to queue False = drop

protected:
	int getInterrupt();

public:
	void interrupt(int);
};

