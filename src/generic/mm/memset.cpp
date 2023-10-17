#include <mm/memory.hpp>

#include <cstdint.hpp>

extern "C" void *memset(void *start, u8 value, u64 num) {
	return Memset(start, value, num);
}

void *Memset(void *start, u8 value, u64 num) {
	for (u64 i = 0; i < num; i++) {
		*(u8*)((u64)start + i) = value;
	}


	return start;
}
