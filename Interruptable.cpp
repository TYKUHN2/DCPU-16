#include "Interruptable.h"

int Interruptable::getInterrupt()
{
	return interruptQueue[--interrupts]; //Pull message and essentially pop it (trigger)
}

void Interruptable::interrupt(int msg) //Generate interrupt and add to queue
{
	if (interruptHook(msg))
	{
		if (interrupts == 256)
		{
			overflow();
			return;
		}

		interruptQueue[interrupts++] = msg;
	}
}
