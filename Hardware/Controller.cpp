#include "Controller.h"
#include "../Display/Debug.h"

constexpr HWClass controller = {
	HWClass::HUMAN_INTERFACE,
	(int)HWSubclass::CONTROLLER		//Controller subclass
};

constexpr HWAPI controllerapi = {
	0xE,	//Subclass based API
	false,	//No extended API
	false,	//Not memory mapped
	false,	//No reset command
	true	//Generates interrupts
};

constexpr uint16_t uniqueid = 0xDACA;
constexpr uint16_t vers		= 0x0001;

constexpr HWType dacacontroller = {
	controller,
	controllerapi,
	uniqueid
};

enum class Commands {
	POLL_DEVICE,
	POLL_ANALOG,
	POLL_DIGITAL,
	ENABLE_INTERRUPTS
};

Controller::Controller(Processor * proc) : Peripheral(proc, Vendors::VEXXAN_PERFORMANCE, dacacontroller, vers) {

}

void Controller::pollAnalog() {
	Debug::print("DACA cannot poll analog ports, not implemented");
}

void Controller::pollDigital() {
	Debug::print("DACA cannot poll digital ports, not implemented");
}

void Controller::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::POLL_DEVICE:
		pollAnalog();
		pollDigital();
		return;
	case Commands::POLL_ANALOG:
		pollAnalog();
		return;
	case Commands::POLL_DIGITAL:
		pollDigital();
		return;
	case Commands::ENABLE_INTERRUPTS:
		msg = parent->registers.b;
	}
}
