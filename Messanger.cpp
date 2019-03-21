#include "Messanger.h"

void Messanger::message(Messagable * target) {
	if (msg != 0)
		target->interrupt(msg);
}
