#include "Executable.h"
#include "Byte_Order.h"

Executable::Executable(std::string path) : file(path)
{
}


Executable::~Executable()
{
}

void Executable::load() { //Loads the executable and returns it. Throws on error.
	if (data != nullptr)
	{
		delete data;
	}

	std::FILE * handle = fopen(file.c_str(), "rb");
	
	if (handle == NULL) {
		throw FileError::NO_LOAD;
	}

	fseek(handle, 0, SEEK_END);

	size = ftell(handle);

	if (size % 2 == 1 || size > 131072) {
		throw FileError::INVALID;
	}

	rewind(handle);

	data = new uint8_t[size];

	fread(data, 1, size, handle);
	fclose(handle);

#ifdef BYTE_ORDER
	if (BYTE_ORDER == LITTLE_ENDIAN) {
		swapBO((char*)data, 2, size / 2);
	}
#else
	#if __BYTE_ORDER__ == __BIG_ENDIAN__
		swapBO(storage, 2, size / 2);
	#endif
#endif
}
