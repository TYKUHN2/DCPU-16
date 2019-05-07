#pragma once
#include "Peripheral.h"
#include "Buffer.h"
#include "Messanger.h"

enum class KeyboardMode {
	SMART_TEXT,
	GENERIC
};

class Buffer;

class Keyboard : public Peripheral, public Messanger {
	Buffer buffer;

	KeyboardMode mode = KeyboardMode::SMART_TEXT;

public:
	Keyboard(Processor*);
	
	void interrupt(uint16_t) override;
};

