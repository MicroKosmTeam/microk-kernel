#include <mm/memory.hpp>

#include <cstdint.hpp>

void *Memclr(void *start, usize num) {
	if (!(num % sizeof(u64))) {
		return Memset64(start, 0, num);
	}

	if (!(num % sizeof(u32))) {
		return Memset32(start, 0, num);
	}

	if (!(num % sizeof(u16))) {
		return Memset16(start, 0, num);
	}

	return Memclr(start, num);
}
