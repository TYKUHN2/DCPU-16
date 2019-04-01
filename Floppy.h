#pragma once
#include "Storage.h"

class Floppy : public Storage, public Messanger {
	enum class State {
		NO_MEDIA,
		READY,
		READY_WP,
		BUSY
	};

	enum class Error {
		NONE,
		BUSY,
		NO_MEDIA,
		PROTECTED,
		EJECTED,
		BAD_SECTOR,
		BROKEN
	};

	State state = State::READY;
	Error error = Error::NONE;

public:
	enum class Density {
		LOW,
		HIGH
	};

	Floppy(Processor*, Floppy::Density, uint8_t, std::string);

	void interrupt(uint16_t) final;
};
