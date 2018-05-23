#include "Debug.h"
#include <cstdio>

void Debug::print(std::string txt)
{
	printf("%s\n", txt.c_str());
}

void Debug::print(unsigned int val)
{
	printf("%X\n", val);
}
