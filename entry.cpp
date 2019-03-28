#include "Processor.h" //Include processor type to construct initial device
#include "Executable.h" //Include executable type to load inital ROM
#include "Result.h" //Include emulator result codes
#include <thread>
#include <string>
#include "Speaker.h"
#include "config.h"
//#include "LEM1802.h"

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

	Executable rom{ "./rom.bin" };

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

	//LEM1802 monitor{ &main };

	Speaker speaker{ &main };

	std::chrono::duration<float, std::ratio<1>> period{ 1 / clock };

	while (true)
	{
		Ticking::doTick();
		std::this_thread::sleep_for(period);
	}

	return SUCCESS;
}