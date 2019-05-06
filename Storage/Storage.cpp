#include "Storage.h"
#include <filesystem>
#include <cstring>

#ifdef _WIN32
#define FILESYSTEM std::filesystem
#else
#define FILESYSTEM std
#endif

constexpr std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary;

Storage::Storage(Processor * proc, Vendors vendor, HWType type, uint16_t vers, std::string dir, std::string id) : Peripheral(proc, vendor, type, vers) {
	FILESYSTEM::path fullpath = FILESYSTEM::current_path() / dir / id;
	fullpath.replace_extension("disk");

	file.open(fullpath, mode);
}

Storage::~Storage() {
	file.close();
}

void Storage::read(uint16_t sector, uint16_t offset) {
	uint16_t maxsectors = sectors * tracks * surfaces;

	if (sector > maxsectors)
		throw Storage::Error::OUT_OF_BOUNDS;

	uint16_t max = offset + 512;
	if (max < offset)
		max = 512 - max;
	else
		max = 512;

	uint16_t * tgt = parent->memory + offset;

	file.read((char *)tgt, max);
	if (file.eofbit) {
		std::streamsize numin = file.gcount();

		memset((void*)(tgt + numin), 0, max - numin);
	}
	else if (file.badbit)
		throw Storage::Error::CRITICAL;
	else if (file.failbit)
		throw Storage::Error::UNKNOWN;
}

void Storage::write(uint16_t sector, uint16_t offset) {
	if (write_protect)
		throw Storage::Error::PROTECTED;

	uint16_t maxsectors = sectors * tracks * surfaces;

	uint16_t * src = parent->memory + offset;

	uint16_t max = offset + 512;
	if (max < offset)
		max = 512 - max;
	else
		max = 512;

	if (sector > maxsectors)
		throw Storage::Error::OUT_OF_BOUNDS;

	file.write((char *)src, max);
	
	if (file.badbit)
		throw Storage::Error::CRITICAL;
	else if (file.failbit)
		throw Storage::Error::UNKNOWN;
}
