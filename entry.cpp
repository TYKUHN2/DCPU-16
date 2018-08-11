#include "Processor.h" //Include processor type to construct initial device
#include "Executable.h" //Include executable type to load inital ROM
#include "Result.h" //Include emulator result codes
#include <thread>
#include <string>
#include "LEM1802.h"

#define DEFAULT_CLOCK 1000 //Set clock speed for default processor in Hz

int main(int argc, char* args)
{
	float clock = DEFAULT_CLOCK;
	
	if (argc > 1)
	{
		try
		{
			std::string input{ args[1] };
			clock *= std::stof(input);
		}
		catch (std::exception e)
		{
		}
	}

	Executable rom{ "./rom.dcpu" };

	try {
		rom.load();
	}
	catch (FileError e) {
		switch (e) {
		case FileError::NO_LOAD:
			return NO_ROM;
		case FileError::INVALID:
			return INVALID_ROM;
		}
	}

	Processor main{ rom };

	LEM1802 monitor{ &main };

	std::chrono::duration<float, std::ratio<1>> period{ 1 / clock };

	while (true)
	{
		Ticking::doTick();
		std::this_thread::sleep_for(period);
	}

	return SUCCESS;
}