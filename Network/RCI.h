#pragma once
#include "Transmitter.h"
#include "../Hardware/Messanger.h"

class RCI : public Transmitter, public Messanger {
	uint16_t recvmsg = 0x0;
	uint16_t sendmsg = 0x0;

	Packet buffer;

protected:
	void receive(Packet) override;

public:
	RCI(Processor*);

	void interrupt(uint16_t) override;
};

