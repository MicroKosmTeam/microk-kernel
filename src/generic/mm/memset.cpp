#include <mm/memory.hpp>
#include <stddef.h>
#include <stdint.h>

extern "C" void *memset(void *start, uint8_t value, uint64_t num) {
	return Memset(start, value, num);
}

void *Memset(void *start, uint8_t value, uint64_t num) {
	for (uint64_t i = 0; i < num; i++) {
		*(uint8_t*)((uint64_t)start + i) = value;
	}


	return start;
}
