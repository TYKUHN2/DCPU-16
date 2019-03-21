#include "Processor.h"
#include <cstring>
#include "Instructions.h"
#include "Peripheral.h"
#include "Debug.h"

#define PUSH(val) memory[--SP] = val
#define POP memory[SP++]

enum class Error {
	SILENT,
	DEVICE_OVERLOAD,
	INVALID_DEVICE
};

void Processor::validateDevice(uint16_t devicenum) {
	if (devicenum >= devicesLen) {
		crashed = true;
		throw Error::INVALID_DEVICE;
	}
}

Processor::Processor(Executable rom) //Initializes a new processor. Expects null-terminated endian-fixed ROM input
{
	std::memcpy(memory, rom.data, rom.size);
}

uint16_t Processor::getValue(uint8_t op) //Gets value represented by an operand
{
	switch (op)
	{
	case 0x0:
		return registers.a;
	case 0x1:
		return registers.b;
	case 0x2:
		return registers.c;
	case 0x3:
		return registers.x;
	case 0x4:
		return registers.y;
	case 0x5:
		return registers.z;
	case 0x6:
		return registers.i;
	case 0x7:
		return registers.j;

	case 0x8:
	case 0x9:
	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
	{
		uint16_t ptr = getValue(op - 0x08);
		return memory[ptr];
	}

	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	{
		debt++;
		uint16_t ptr = getValue(op - 0x10);
		ptr += memory[PC++];
		return memory[ptr];
	}
	
	case 0x18:
		return memory[SP++];
	case 0x19:
		return memory[SP];
	case 0x1A:
	{
		debt++;
		uint16_t ptr = SP + memory[PC++]; //Read extra word and skip extra word on for next execution
		return memory[ptr];
	}

	case 0x1B:
		return SP;
	case 0x1C:
		return PC;
	case 0x1D:
		return EX;

	case 0x1E:
	{
		debt++;
		uint16_t ptr = memory[PC++];
		return memory[ptr]; //Read extra word and skip extra word on for next execution
	}
	
	case 0x1F:
	{
		debt++;
		return memory[PC++]; //Read extra word and skip extra word on for next execution
	}

	default:
		return 0xFFFF + op - 0x20; //Literal value from -1 to 30 using opcode and opcode offset to generate
	}
}

uint16_t * Processor::getDest(uint8_t op)
{
	switch (op)
	{
	case 0x0:
		return &registers.a;
	case 0x1:
		return &registers.b;
	case 0x2:
		return &registers.c;
	case 0x3:
		return &registers.x;
	case 0x4:
		return &registers.y;
	case 0x5:
		return &registers.z;
	case 0x6:
		return &registers.i;
	case 0x7:
		return &registers.j;

	case 0x8:
	case 0x9:
	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
	{
		uint16_t ptr = getValue(op - 0x08);
		return memory + ptr;
	}

	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
	case 0x15:
	case 0x16:
	case 0x17:
	{
		debt++;
		uint16_t ptr = getValue(op - 0x10);
		ptr += memory[PC++];
		return memory + ptr;
	}

	case 0x18:
		return memory + --SP;
	case 0x19:
		return memory + SP;
	case 0x1A:
	{
		debt++;
		uint16_t ptr = SP + memory[PC++]; //Read extra word and skip extra word on for next execution
		return memory + ptr;
	}

	case 0x1B:
		return &SP;
	case 0x1C:
		return &PC;
	case 0x1D:
		return &EX;

	case 0x1E:
	{
		debt++;
		uint16_t ptr = memory[PC++];
		return memory + ptr; //Read extra word and skip extra word on for next execution
	}

	case 0x1F:
		debt++;
		PC++; //Skip next word even though we aren't using it
	default:
		throw Error::SILENT; //Signal to processor we can stop executing.
	}
}

uint16_t Processor::peek(uint8_t op) //Get's value without incrementing PC incase of extra word (for read then set purposes)
{
	uint16_t temp = PC;
	char temp2 = debt;

	uint16_t val = getValue(op);

	PC = temp;
	debt = temp2;
	return val;
}

void Processor::conditionalSkip() //Conditional evaluated false, perform chain skip
{
	while (true) {
		uint16_t next = getValue(0x1E); //Get next word using shortcut 0x1E = next word also advances debt

		char first = next >> 10;
		char second = (next >> 5) & 0b11111;
		char opcode = next & 0b11111;

		if (opcode > 0x0F && opcode < 0x18)
		{
			char temp = debt;
			uint16_t temp2 = SP;

			getValue(first);
			getValue(second);

			debt = temp;
			SP = temp2;
		}
		else
		{
			return;
		}
	}
}

void Processor::doubleParam(uint8_t first, uint8_t second, uint8_t opcode) //Process dual-operand instruction
{
	/*General operation execution sequence
		1) Read values from relevant operands
		2) Execute evaluation algorithms
		3) Write effects (EX, I, J) (Violates Errata, could cause errors)
		4) Read destination operands (as relevant)
		5) Write to destination (or drop if literal) */

	using namespace Instructions;

	switch (opcode) {
	case SET:
	{
		debt++;
		uint16_t a = getValue(first);

		uint16_t * dest = getDest(second);

		*dest = a;
		return;
	}

	case ADD:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		uint16_t result = a + b;

		*getDest(second) = result;

		if (result < a)
		{
			EX = 0x1;
		}
		else
		{
			EX = 0x0;
		}
		return;
	}

	case SUB:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		uint16_t result = b - a;

		*getDest(second) = result;

		if (result > b)
		{
			EX = 0xFFFF;
		}
		else
		{
			EX = 0x0;
		}
		return;
	}

	case MUL:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		uint16_t result = b * a;

		*getDest(second) = result;

		EX = ((a*b) >> 16) & 0xFFFF;
		return;
	}

	case MLI:
	{
		debt += 2;
		int16_t a = getValue(first);
		int16_t b = peek(second);

		int16_t result = a * b;

		*getDest(second) = result;

		EX = ((a*b) >> 16) & 0xFFFF;
		return;
	}

	case DIV:
	{
		debt += 3;
		uint16_t a= getValue(first);
		uint16_t b = peek(second);

		if (a == 0) {
			*getDest(second) = 0;
			EX = 0;
		}
		else
		{
			uint16_t result = b / a;

			*getDest(second) = result;

			EX = ((b << 16) / a) & 0xFFFF;
		}

		return;
	}

	case DVI:
	{
		debt += 3;
		int16_t a = getValue(first);
		int16_t b = peek(second);

		if (a == 0) {
			*getDest(second) = 0;
			EX = 0;
		}
		else
		{
			int16_t result = b / a;

			*getDest(second) = result;

			EX = ((b << 16) / a) & 0xFFFF;
		}

		return;
	}

	case MOD:
	{
		debt += 3;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		uint16_t * dest = getDest(second);

		if (a == 0) {
			*dest = 0;
		}
		else
		{
			*dest = b % a;
		}
		return;
	}

	case MDI:
	{
		debt += 3;
		int16_t a = getValue(first);
		int16_t b = peek(second);

		uint16_t * dest = getDest(second);

		if (a == 0) {
			*dest = 0;
		}
		else
		{
			*dest = b % a;
		}
		return;
	}

	case AND:
	{
		debt++;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		*getDest(second) = b & a;

		return;
	}

	case BOR:
	{
		debt++;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		*getDest(second) = b | a;

		return;
	}

	case XOR:
	{
		debt++;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		*getDest(second) = b ^ a;

		return;
	}

	case SHR:
	{
		debt++;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		EX = ((b << 16) >> a) & 0xFFFF;

		*getDest(second) = b >> a;

		return;
	}

	case ASR:
	{
		debt++;
		uint16_t a = getValue(first);
		int16_t b = peek(second);

		EX = ((b << 16) >> a) & 0xFFFF;

		*getDest(second) = b >> a;

		return;
	}

	case SHL:
	{
		debt++;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		*getDest(second) = b << a;

		EX = ((b << a) >> 16) & 0xFFFF;

		return;
	}

	case IFB:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = getValue(second);

		if ((b & a) == 0) {
			conditionalSkip();
		}
		return;
	}

	case IFC:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = getValue(second);

		if (b & a) {
			conditionalSkip();
		}
		return;
	}

	case IFE:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = getValue(second);

		if (b != a) {
			conditionalSkip();
		}
		return;
	}

	case IFN:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = getValue(second);

		if (b == a) {
			conditionalSkip();
		}
		return;
	}

	case IFG:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = getValue(second);

		if (b <= a) {
			conditionalSkip();
		}
		return;
	}

	case IFA:
	{
		debt += 2;
		int16_t a = getValue(first);
		int16_t b = getValue(second);

		if (b <= a) {
			conditionalSkip();
		}
		return;
	}

	case IFL:
	{
		debt += 2;
		uint16_t a = getValue(first);
		uint16_t b = getValue(second);

		if (b >= a) {
			conditionalSkip();
		}
		return;
	}

	case IFU:
	{
		debt += 2;
		int16_t a = getValue(first);
		int16_t b = getValue(second);

		if (b >= a) {
			conditionalSkip();
		}
		return;
	}

	case ADX:
	{
		debt += 3;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		uint16_t result = b + a + EX;

		*getDest(second) = result;

		if (result < b)
		{
			EX = 1;
		}
		else
		{
			EX = 0;
		}
		return;
	}

	case SBX:
	{
		debt += 3;
		uint16_t a = getValue(first);
		uint16_t b = peek(second);

		uint16_t result = b - a + EX;

		*getDest(second) = result;

		if (result < b)
		{
			EX = 1;
		}
		else
		{
			EX = 0;
		}
		return;
	}

	case STI:
	{
		debt += 2;
		uint16_t a = getValue(first);

		*getDest(second) = a;

		registers.i++;
		registers.j++;
		return;
	}

	case STD:
	{
		debt += 2;
		uint16_t a = getValue(first);

		*getDest(second) = a;

		registers.i--;
		registers.j--;
		return;
	}
	}
}

void Processor::singleParam(uint8_t param, uint8_t opcode) //Process single-operand instruction
{
	/*General operation execution sequence
	1) Read values from relevant operands
	2) Execute evaluation algorithms
	3) Write effects (EX, I, J) (Violates Errata, could cause errors)
	4) Read destination operands (as relevant)
	5) Write to destination (or drop if literal) */

	using namespace Instructions;

	switch (opcode)
	{
	case JSR:
	{
		debt += 3;
		uint16_t temp = getValue(param);
		PUSH(PC);
		PC = temp;
		return;
	}

	case INT: //FIX ACCORDING TO SPEC (SWI)
		debt += 4;
		interrupt(getValue(param));
		break;

	case IAG:
		debt++;
		*getDest(param) = IA;
		return;

	case IAS:
		debt++;
		IA = getValue(param);
		return;

	case RFI:
		debt += 3;
		registers.a = POP;
		PC = POP;
		iq = false;
		return;

	case IAQ:
		debt += 2;
		if (getValue(param)) {
			iq = true;
		}
		else
		{
			iq = false;
		}
		return;

	case HWN:
		debt += 2;
		*getDest(param) = devicesLen;
		return;

	case HWQ:
	{
		debt += 4;

		uint16_t id = getValue(param);
		validateDevice(id);

		Peripheral * device = devices[id];

		registers.a = device->type & 0xFFFF;
		registers.b = device->type >> 16;

		registers.c = device->version;

		registers.x = device->manufacturer & 0xFFFF;
		registers.y = device->manufacturer >> 16;
		return;
	}

	case HWI:
	{
		debt += 4;

		uint16_t id = getValue(param);
		validateDevice(id);

		interruptDevice(id);
		held = true;
		return;
	}

	case LOG:
		debt++;
		log(getValue(param));
		return;

	case BRK:
		brk(getValue(param));
		return;

	case HLT:
		mode = PowerMode::SLEEP;
		return;
	}
}

void Processor::tick() //Process next instruction and return cycles to wait
{
	if (crashed || mode == PowerMode::SLEEP)
	{
		return;
	}

	if (debt)
	{
		debt--;
		return;
	}
	else if (queuedInterrupt != 0xFFFF)
	{
		devices[queuedInterrupt]->interrupt(0);
		queuedInterrupt = 0xFFFF;
	}
	else if (held)
	{
		return;
	}

	if (iq == false) //If we have an interrupt and queueing is off
	{
		int msg = getInterrupt();

		if (IA != 0) //Execute interrupt handler
		{
			iq = true;
			PUSH(PC);
			PUSH(registers.a);
			PC = IA;
			registers.a = msg;
		}
	}

	uint16_t cmd = memory[PC++];

	uint8_t first = cmd >> 10;

	if (cmd & 0b11111) {
		uint8_t second = (cmd >> 5) & 0b11111;
		uint8_t opcode = cmd & 0b11111;

		try //Catch silent errors
		{
			doubleParam(first, second, opcode); //Process two operand instruction
		}
		catch ([[maybe_unused]] Error e)
		{
		}
	}
	else {
		char opcode = (cmd >> 5) & 0b11111;

		try //Catch silent errors
		{
			singleParam(first, opcode); //Process one operand instruction
		}
		catch ([[maybe_unused]] Error e)
		{
		}
	}
};

void overflow(uint16_t * mem) //Randomly corrupts processor memory
{

}

void Processor::release()
{
	held = false;
}

void Processor::connect(Peripheral * connectee)
{
	if (devicesLen == 65535)
	{
		throw Error::DEVICE_OVERLOAD;
	}
	
	devices[devicesLen++] = connectee;
}

bool Processor::inDebt() {
	return debt > 0;
}

void Processor::interruptDevice(int addr)
{
	queuedInterrupt = addr;
	held = true;
}

void Processor::charge(int amnt)
{
	debt += amnt;
}

void Processor::log(int val)
{
	Debug::print(val);
}

void Processor::brk(int val)
{
}
