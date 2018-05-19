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
	Executable(std::string);
	~Executable();

	char* load();
};

