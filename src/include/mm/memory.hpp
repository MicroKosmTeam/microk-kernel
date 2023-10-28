#pragma once
#include <cstdint.hpp>

#define MEMMAP_USABLE                 0
#define MEMMAP_RESERVED               1
#define MEMMAP_ACPI_RECLAIMABLE       2
#define MEMMAP_ACPI_NVS               3
#define MEMMAP_BAD_MEMORY             4
#define MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define MEMMAP_KERNEL_AND_MODULES     6
#define MEMMAP_FRAMEBUFFER            7

void *Memset(void *start, u8 value, u64 num);
void *Memcpy(void *dest, void *src, usize n);
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

extern const u8 __KernelBinaryEssentialStart;
extern const u8 __KernelBinaryEssentialEnd;
extern const u8 __KernelBinaryTextStart;
extern const u8 __KernelBinaryTextEnd;
extern const u8 __KernelBinaryRODataStart;
extern const u8 __KernelBinaryRODataEnd;
extern const u8 __KernelBinaryDataStart;
extern const u8 __KernelBinaryDataEnd;


namespace MEM {
	struct MMapEntry {
		uptr AddressBase;
		uptr Length;
		u8 Type;
	};

	inline const char *MemoryTypeToString(u8 type) {
		const char *memTypeStrings[] = {
			"Usable",
			"Reserved",
			"ACPI Reclaimable",
			"ACPI NVS",
			"Bad",
			"Bootloader reclaimable",
			"Kernel and modules",
			"Framebuffer"
		};

		return memTypeStrings[type];
	}

	void Init();
	void InvokeOOM();
}
