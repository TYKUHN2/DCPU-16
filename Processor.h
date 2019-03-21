#pragma once
#include "Executable.h"
#include "Registers.h"
#include "Messagable.h"
#include "Messanger.h"
#include "Ticking.h"

class Peripheral;

enum class PowerMode
{
	FULL,
	REDUCED,
	PULSE,
	SLEEP,
	OFF
};

class RACM;

class Processor : public Messagable, public Ticking, public Messanger
{
	friend RACM;

	uint16_t PC = 0;
	uint16_t SP = 0;
	uint16_t EX = 0;
	uint16_t IA = 0;

	bool held = false;
	bool iq = false;
	bool crashed = false;

	int devicesLen = 0;
	Peripheral * devices[65535];

	int queuedInterrupt = 0xFFFF;

	char debt = 0;

	void doubleParam(uint8_t, uint8_t, uint8_t);
	void singleParam(uint8_t, uint8_t);

	void validateDevice(uint16_t);

	uint16_t getValue(uint8_t);
	uint16_t * getDest(uint8_t);
	uint16_t peek(uint8_t);

	void conditionalSkip();

public:
	Registers registers;
	uint16_t memory[65536];
	
	Processor(Executable);

	bool inDebt();
	void release();
	void charge(int);

	void connect(Peripheral *);

	void tick() override;

	void interruptDevice(int); //Probably to be replaced with more direct access

	void log(int);
	void brk(int);
};

