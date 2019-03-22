#pragma once
#include "Wired.h"

class SSI : public Wired, public Messanger {
	uint16_t recvmsg;
	uint16_t sendmsg;

	uint32_t buffer;
	bool buffered = false;
	bool overflow = false;

	uint32_t queue;
	bool queued = false;

	uint8_t size = 4;

public:
	SSI(Processor*);

	void interrupt(uint16_t) override;
	void receive(uint32_t, Port*) override;
	void onConnect(Port*) override;
};
