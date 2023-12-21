#include <sys/loader.hpp>
#include <elf.h>
#include <sys/elf.hpp>
#include <sys/shebang.hpp>
#include <sys/printk.hpp>

usize LoadExecutableFile(u8 *data, usize size) {
	/* Checking for the correct signature */
	
	if (data[0] == ELFMAG0 && data[1] == ELFMAG1 && data[2] == ELFMAG2 && data[3] == ELFMAG3)
		return LoadELF(data, size);
	else if (data[0] == '#' && data[1] == '!')
		return LoadShebang(data, size);
	else
		return -1;
}
