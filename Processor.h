#pragma once

#include "Registers.h"
#include "Interruptable.h"
#include "Hardware.h"

enum class PowerMode
{
	FULL,
	REDUCED,
	PULSE,
	SLEEP,
	OFF
};

class Processor : public Interruptable, public Ticking
{
	PowerMode mode = PowerMode::OFF;

	int PC = 0;
	int SP = 0;
	int EX = 0;
	int IA = 0;

	bool held = false;
	bool iq = false;

	int devicesLen = 0;
	Hardware * devices[65535];

	int queuedInterrupt = 0xFFFF;

	char debt = 0;

	void doubleParam(char, char, char);
	void singleParam(char, char);

	int getValue(char);
	int * getDest(char);
	int peek(char);

	void conditionalSkip();

public:
	Registers registers;
	int memory[65536];
	
	Processor(char*);

	bool inDebt();
	void release();

	void connect(Hardware *);

	void process() override;

	void interruptDevice(int); //Probably to be replaced with more direct access

	void log(int);
	void brk(int);
};

