#include <mm/memory.hpp>

#include <cstdint.hpp>

extern "C" void *memset(void *start, u8 value, usize num) {
	return Memset(start, value, num);
}

void *Memset(void *start, u8 value, usize num) {
	num /= sizeof(u8);

	for (usize i = 0; i < num; i++) {
		*(u8*)((uptr)start + i) = value;
	}


	return start;
}

void *Memset16(void *start, u16 value, usize num) {
	num /= sizeof(u16);

	for (usize i = 0; i < num; i++) {
		*(u16*)((uptr)start + i) = value;
	}


	return start;
}

void *Memset32(void *start, u32 value, usize num) {
	num /= sizeof(u32);

	for (usize i = 0; i < num; i++) {
		*(u32*)((uptr)start + i) = value;
	}


	return start;
}

void *Memset64(void *start, u64 value, usize num) {
	num /= sizeof(u64);

	for (usize i = 0; i < num; i++) {
		*(u64*)((uptr)start + i) = value;
	}


	return start;
}
