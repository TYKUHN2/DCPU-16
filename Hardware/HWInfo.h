#pragma once
#include "Vendors.h"

struct HWClass {
	enum {
		EXPANSION_BUS,
		INTEGRATED,
		SENSOR,
		HUMAN_INTERFACE,
		STORAGE,
		MEMORY,
		MULTIMEDIA,
		RASTER_DISPLAY,
		VECTOR_DISPLAY,
		PROCESSOR,
		RESERVED_1,
		RESERVED_2,
		GENERIC_OUTPUT,
		WIRELESS,
		WIRED,
		MISCELLANEOUS
	};

	int hwclass : 4;
	int subclass : 4;
};

enum class HWSubclass {
	CPU = 1,
	TIMER = 2,
	RNG = 3,
	RTC = 4,
	ROM = 7,
	KEYBOARD = 0,
	CONTROLLER = 1,
	FLOPPY_DRIVE = 15,
	DMA = 1,
	MMU = 8,
	NON_VOLATILE = 4,
	TEXT_COMMAND = 0,
	TEXT_MEMORY = 1,
	TEXT_COMMAND_COLOR = 2,
	TEXT_MEMORY_COLOR = 3,
	PIXEL_COMMAND = 4,
	PIXEL_MEMORY = 5,
	PIXEL_COMMAND_COLOR = 6,
	PIXEL_MEMORY_COLOR = 7,
	PIXEL_MEMORY_CARD = 8,
	VECTOR = 2,
	HOLOGRAHPIC = 3,
	PACKET_RADIO = 0,
	PARALLEL = 0,
	SERIAL = 1,
	THRUST_CONTROLLER = 7
};

struct HWAPI {
	enum {
		SUBCLASS = 0xE,
		NOT_STANDARD
	};

	uint8_t id : 4;
	bool extended : 1;
	bool memory : 1;
	bool reset : 1;
	bool interrupts : 1;
};

struct HWType {
	HWClass hwclass;
	HWAPI api;
	uint16_t id;
};

struct HWInfo {
	HWType type;
	Vendors vendor;
	uint16_t vers;
};
