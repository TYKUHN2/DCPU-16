#include "Executable.h"
#include "Byte_Order.h"

Executable::Executable(std::string path) : file(path)
{
}


Executable::~Executable()
{
}

char * Executable::load() { //Loads the executable and returns it. Throws on error.
	std::FILE * handle = fopen(file.c_str(), "r");
	
	if (handle == NULL) {
		throw FileError::NO_LOAD;
	}

	fseek(handle, 0, SEEK_END);

	int size = ftell(handle);

	if (size % 2 == 1 || size > 131072) {
		throw FileError::INVALID;
	}

	rewind(handle);

	char * storage = new char[size + 1];
	storage[size] = 0x0;

	fread(storage, 1, size, handle);
	fclose(handle);

#ifdef BYTE_ORDER
	if (BYTE_ORDER == BIG_ENDIAN) {
		swapBO(storage, 2, size / 2);
	}
#else
	#if __BYTE_ORDER__ == __BIG_ENDIAN__
		swapBO(storage, 2, size / 2);
	#endif
#endif

	return storage;
}
