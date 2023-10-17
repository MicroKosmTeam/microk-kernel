
#include <cstdint.hpp>
#include <mm/memory.hpp>

extern "C" void *memcpy(void *dest, void *src, usize n) {
	return Memcpy(dest, src, n);
}

void *Memcpy(void *dest, void *src, usize n) {
	char *csrc = (char *)src;
	char *cdest = (char *)dest;

	for (usize i=0; i<n; i++) cdest[i] = csrc[i];

	return dest;
}
