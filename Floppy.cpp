#include "Floppy.h"

constexpr HWClass floppydrive {
	HWClass::STORAGE,
	(int)HWSubclass::FLOPPY_DRIVE
};

constexpr uint16_t uniqueid	= 0x24C5;
constexpr uint16_t vers		= 0x000B;

constexpr HWAPI floppyapi{
	0xD,		//Random API ID
	false,		//No extended API
	true,		//Memory mapped
	false,		//No reset command
	true		//Generates interrupts
};

constexpr HWType floppy {
	floppydrive,
	floppyapi,
	uniqueid
};

enum class Commands {
	POLL,
	INTERRUPT,
	READ,
	WRITE,
	GEOMETRY
};

Floppy::Floppy(Processor * proc, Floppy::Density density, uint8_t sides, std::string id) : Storage(proc, Vendors::MACKAPAR_MEDIA, floppy, vers, "floppy", id) {
	if (sides > 2)
		sides = 2;
	else if (sides < 1)
		sides = 1;

	surfaces = sides;

	switch (density) {
	case Floppy::Density::LOW:
		tracks = 20;
		sectors = 4;
		return;
	case Floppy::Density::HIGH:
		tracks = 32;
		sectors = 7;
	}
}

void Floppy::interrupt(uint16_t command) {
	switch ((Commands)command) {
	case Commands::POLL:
		parent->registers.b = (uint16_t)state;
		parent->registers.c = (uint16_t)error;
		error = Error::NONE;
		return;
	case Commands::INTERRUPT:
		msg = parent->registers.x;
		return;
	case Commands::READ:
		try {
			read(parent->registers.x, parent->registers.y);
			parent->registers.b = 1;
		}
		catch (Storage::Error e) {
			parent->registers.b = 0;

			switch (e) {
			case Storage::Error::OUT_OF_BOUNDS:
				error = Error::BROKEN;
				break;
			default:
				error = Error::BAD_SECTOR;
			}

			message(parent);
		}
		return;
	case Commands::WRITE:
		try {
			write(parent->registers.x, parent->registers.y);
			parent->registers.b = 1;
		}
		catch (Storage::Error e) {
			parent->registers.b = 0;

			switch (e) {
			case Storage::Error::PROTECTED:
				error = Error::PROTECTED;
				break;
			case Storage::Error::OUT_OF_BOUNDS:
				error = Error::BROKEN;
				break;
			default:
				error = Error::BAD_SECTOR;
			}

			message(parent);
		}
		return;
	case Commands::GEOMETRY:
		parent->registers.x = surfaces;
		parent->registers.y = tracks;
		parent->registers.z = sectors;
	}
}
