#include <stddef.h>
#include <stdint.h>
#include <mm/memory.hpp>

extern "C" void *memcpy(void *dest, void *src, size_t n) {
	return Memcpy(dest, src, n);
}

void *Memcpy(void *dest, void *src, size_t n) {
	char *csrc = (char *)src;
	char *cdest = (char *)dest;

	for (size_t i=0; i<n; i++) cdest[i] = csrc[i];

	return dest;
}
