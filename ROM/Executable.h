#pragma once
#include <string>

enum class FileError {
	NO_LOAD,
	INVALID
};

class Executable
{
	std::string file;

public:
	uint8_t * data = nullptr;

	int size = -1; //Updated every load, only -1 if never loaded

	Executable(std::string);
	~Executable();

	void load();
};

