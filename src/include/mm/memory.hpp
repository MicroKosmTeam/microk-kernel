#pragma once
#include <stdint.h>
#include <stddef.h>

void *Memcpy(void *dest, void *src, size_t n);
void *Memset(void *start, uint8_t value, uint64_t num);
int Memcmp(const void* buf1, const void* buf2, size_t count);
void *Memmove(void *dest, const void *src, size_t n);

void *Malloc(size_t size);
void Free(void *p);

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void* p);
void operator delete(void* p, size_t size);
void operator delete[](void* p);
void operator delete[](void* p, size_t size);

namespace MEM {
	#define MEMMAP_USABLE                 0
	#define MEMMAP_RESERVED               1
	#define MEMMAP_ACPI_RECLAIMABLE       2
	#define MEMMAP_ACPI_NVS               3
	#define MEMMAP_BAD_MEMORY             4
	#define MEMMAP_BOOTLOADER_RECLAIMABLE 5
	#define MEMMAP_KERNEL_AND_MODULES     6
	#define MEMMAP_FRAMEBUFFER            7

	struct MMapEntry {
		uint64_t base;
		uint64_t length;
		uint64_t type;
	};

	void Init();
	void DisplayRam();
}
