#include "RCI.h"
#include "Debug.h"

HWAPI RCIapi = {
	0x00,	//Standard API 0
	false,	//No extended API
	true,	//Memory mapped
	false,	//No reset command
	true	//Generates interrupts
};

constexpr uint16_t uniqueid = 0x90A5;
constexpr uint16_t vers		= 0x0010;

enum class Commands {
	QUERY,
	RECEIVE,
	SEND,
	RADIO,
	INTERRUPT
};

union STATUS {
	enum {
		ATTENNA = 0xFFE0,
		INTERNAL = 0xFFF0
	};

	struct {
		bool data : 1;
		bool receiving : 1;
		bool transmitting : 1;
	};

	uint16_t raw;
};

RCI::RCI(Processor * proc) : Transmitter(proc, Vendors::KAI_COMMUNICATIONS, HWSubclass::PACKET_RADIO, RCIapi, uniqueid, vers) {
}

void RCI::receive(Packet packet) {
	buffer = packet;
	msg = recvmsg;
	message(parent);
}

void RCI::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::QUERY:
	{
		parent->registers.a = channel;
		parent->registers.b = getPower();

		STATUS status = {
			buffer.size != 0,
			false,
			false
		};

		parent->registers.c = status.raw;
		return;
	}
	case Commands::RECEIVE:
		parent->registers.b = buffer.size;

		if (buffer.size != 0) {
			uint16_t * tgt = parent->memory + parent->registers.b;
			memcpy(tgt, buffer.data, buffer.size);

			parent->registers.c = 0;

			buffer = Packet{
				nullptr,
				0
			};
		}
		else {
			parent->registers.c = 1;
		}

		return;
	case Commands::SEND:
		Packet data = Packet{
			parent->memory + parent->registers.b,
			(uint8_t)parent->registers.c
		};

		transmit(data);

		parent->registers.c = 0;
		msg = sendmsg;
		message(parent);
		return;
	case Commands::RADIO:
		channel = (uint8_t)parent->registers.b;
		setPower((uint8_t)parent->registers.c);
		parent->registers.c = 0;
		return;
	case Commands::INTERRUPT:
		recvmsg = parent->registers.b;
		sendmsg = parent->registers.c;
	}
}
