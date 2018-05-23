#include "Debug.h"
#include <cstdio>

void Debug::print(sf::String txt)
{
	printf("%s\n", ((std::string)txt).c_str());
}

void Debug::print(unsigned int val)
{
	printf("%X\n", val);
}
