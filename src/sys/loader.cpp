#include <sys/loader.hpp>
#include <sys/elf.hpp>

size_t LoadExecutableFile(uint8_t *data, size_t size) {
	/* Checking for the correct signature */
	if (data[0] != 0x7F || data[1] != 'E' || data[2] != 'L' || data[3] != 'F')
		return LoadELF(data, size);
}
