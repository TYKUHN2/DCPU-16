#include "Processor.h" //Include processor type to construct initial device
#include "Executable.h" //Include executable type to load inital ROM
#include "Result.h" //Include emulator result codes

#define DEFAULT_CLOCK 1000 //Set clock speed for default processor in Hz

int main(int argc, char* args)
{
	Executable rom{ "./rom.dcpu" };

	char * romData = nullptr;

	try {
		romData = rom.load();
	}
	catch (FileError e) {
		switch (e) {
		case FileError::NO_LOAD:
			return NO_ROM;
		case FileError::INVALID:
			return INVALID_ROM;
		}
	}

	Processor main{ romData };

	delete romData;

	return SUCCESS;
}