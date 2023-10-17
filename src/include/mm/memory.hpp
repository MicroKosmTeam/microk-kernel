#pragma once
#include <cstdint.hpp>


void *Memcpy(void *dest, void *src, usize n);
void *Memset(void *start, u8 value, u64 num);
int Memcmp(const void* buf1, const void* buf2, usize count);
void *Memmove(void *dest, const void *src, usize n);

void *Malloc(usize size);
void Free(void *p);

void *operator new(usize size);
void *operator new[](usize size);
void operator delete(void* p);
void operator delete(void* p, usize size);
void operator delete[](void* p);
void operator delete[](void* p, usize size);

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
		uptr base;
		uptr length;
		uptr type;
	};

	void Init();
	void InvokeOOM();
	void DisplayRam();
}
