#pragma once
#include <stdint.h>
#include <stddef.h>

void memcpy(void *dest, void *src, size_t n);
void memset(void *start, uint8_t value, uint64_t num);
int memcmp(const void* buf1, const void* buf2, size_t count);

void *Malloc(size_t size);
void Free(void *p);

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void* p);
void operator delete(void* p, size_t size);
void operator delete[](void* p);
void operator delete[](void* p, size_t size);

namespace MEM {
	void Init();
}
