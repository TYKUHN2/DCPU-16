#include "SSI.h"
#include "Debug.h"

HWAPI SSIapi = {
	0x7,		//Random standard API
	true,		//Has extended API
	true,		//Memory mapped
	false,		//No reset command
	true		//Generates interrupts
};

uint16_t uniqueid	= 0x9027;
uint16_t vers		= 0x0103;

enum class Commands {
	QUERY,
	CONFIGURE,
	RECEIVE,
	TRANSMIT,
	INTERRUPT
};

enum Errors {
	NONE,
	OVRFLOW,
	NOT_OCTET,
	NO_DATA,
	BUSY = NOT_OCTET
};

union INTERRUPTS {
	uint16_t raw;

	struct {
		bool receive : 1;
		bool transmit : 1;
	};
};

union STATUS {
	struct {
		bool busy : 1;
		bool receiving : 1;
		bool data : 1;
		bool transmitting : 1;
		bool free : 1;
		int : 9;
		bool recvint : 1;
		bool sendint : 1;
	};

	uint16_t raw = 0;
};

SSI::SSI(Processor * proc) : Wired(proc, Vendors::KAI_COMMUNICATIONS, HWSubclass::SERIAL, SSIapi, uniqueid, vers) {
}

void SSI::receive(uint32_t data, Port * port) {
	if (buffered)
		overflow = true;

	if (data > (uint32_t)( 2 ^ (8 * size))) {
		overflow = true;
		data = data & 2 ^ (8 * size);
	}

	buffer = data;
	buffered = true;
}

void SSI::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::QUERY:
	{
		STATUS status;

		if (!port.connected)
			status.busy = true;

		if (buffered)
			status.data = true;

		if (queued)
			status.free = false;

		if (recvmsg != 0)
			status.recvint = true;

		if (sendmsg != 0)
			status.sendint = true;

		Debug::print("Networking does not implement delay. Transmitting/Receiving set to false.");

		parent->registers.a = status.raw;
		return;
	}
	case Commands::CONFIGURE:
		size = parent->registers.b + 1;
		Debug::print("Networking does not implement delay. Baud rate ignored");
		return;
	case Commands::RECEIVE:
		parent->registers.a = buffer;
		parent->registers.b = buffer >> 16;

		if (overflow)
			parent->registers.c = Errors::OVRFLOW;
		else if (!buffered)
			parent->registers.c = Errors::NO_DATA;
		else
			parent->registers.c = Errors::NONE;

		buffer = 0;
		buffered = false;
		overflow = false;
		return;
	case Commands::TRANSMIT:
	{
		uint32_t data = parent->registers.b | (parent->registers.c << 16);

		if (port.connected) {
			port.send(data);
			parent->registers.c = Errors::NONE;
			msg = sendmsg;
			message(parent);

			Debug::print("Networking does not implement delay. Queueing immediate interrupt");
		}
		else if (queued) {
			parent->registers.c = Errors::OVRFLOW;
		}
		else {
			queued = true;
			queue = data;
			parent->registers.c = Errors::BUSY;
		}
		return;
	}
	case Commands::INTERRUPT:
		INTERRUPTS config;
		config.raw = parent->registers.b;

		if (config.receive)
			recvmsg = parent->registers.c;
		else
			recvmsg = 0;

		if (config.transmit)
			sendmsg = parent->registers.x;
		else
			sendmsg = 0;
	}
}

void SSI::onConnect(Port * port) {
	if (queued) {
		queued = false;
		port->send(queue);
		msg = sendmsg;
		message(parent);
	}
}
