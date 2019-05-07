#include "RACM.h"
#include "../Display/Debug.h"
#include "../Processor/Processor.h"

constexpr HWAPI RACMapi = {
	HWAPI::NOT_STANDARD,
	false,	//No extended API
	false,	//Not memory mapped
	false,	//No reset command
	false	//Does not generate interrupts
};

constexpr uint16_t uniqueid = 0x90C0;
constexpr uint16_t vers		= 0x0001;

enum class Commands {
	QUERY,
	RESET,
	PIN,
	RADIO
};

enum class RemoteCommands {
	PIN,
	RESET,
	UPLOAD
};

inline bool checkHeader(uint16_t * data) {
	return data[0] == 0xA87C && data[1] == 0x900E;
}

RACM::RACM(Processor * proc) : Wireless(proc, Vendors::KAI_COMMUNICATIONS, HWSubclass::PACKET_RADIO, RACMapi, uniqueid, vers) {
}

void RACM::receive(Packet packet) {
	uint16_t * data = packet.data;

	if (packet.size > 2 &&  checkHeader(data)) {
		switch ((RemoteCommands)data[2]) {
		case RemoteCommands::PIN:
			if (data[3] == pin)
				safemode = true;
			return;
		case RemoteCommands::RESET:
			if (safemode) {
				parent->reset(); //Reset CPU
				Debug::print("RACM cannot reset devices, not implemented");
				//Reset devices!!!
			}
			return;
		case RemoteCommands::UPLOAD:
			uint16_t* tgt = &(parent->memory[0]);

			Debug::print("RACM cannot reset devices, not implemented");
			//Reset devices!!!

			memcpy(tgt, data + 3, packet.size - 3);

			parent->PC = 0;
			parent->registers = { 0, 0, 0, 0, 0, 0, 0, 0 };
			parent->IA = 0;
			parent->SP = 0;
			parent->EX = 0;
			parent->IQ = false;
		}
	}
}

void RACM::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::QUERY:
		Debug::print("RACM cannot read watchdog timer, not implemented");
		parent->registers.a = 0; //Return time to watchdog
		parent->registers.b = pin;
		return;
	case Commands::RESET:
		Debug::print("RACM cannot reset watchdog timer, not implemented");
		//Reset watchdog timer
		return;
	case Commands::PIN:
		pin = parent->registers.b;
		return;
	case Commands::RADIO:
		channel = (uint8_t)parent->registers.b;
		parent->registers.c = 0;
	}
}
