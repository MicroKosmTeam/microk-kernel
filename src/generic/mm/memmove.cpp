#include <mm/memory.hpp>

#include <cstdint.hpp>


extern "C" void *memmove(void *dest, const void *src, usize n) {
	return Memmove(dest, src, n);
}

void *Memmove(void *dest, const void *src, usize n) {
	u8 *pdest = (u8 *)dest;
	const u8 *psrc = (const u8 *)src;

	if (src > dest) {
		for (usize i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	} else if (src < dest) {
		for (usize i = n; i > 0; i--) {
			pdest[i-1] = psrc[i-1];
		}
	}

	return dest;
}
