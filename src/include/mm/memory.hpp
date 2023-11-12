#pragma once
#include <cstdint.hpp>

#define MEMMAP_USABLE                 0x00
#define MEMMAP_RESERVED               0x01
#define MEMMAP_ACPI_RECLAIMABLE       0x02
#define MEMMAP_ACPI_NVS               0x03
#define MEMMAP_BAD_MEMORY             0x04
#define MEMMAP_BOOTLOADER_RECLAIMABLE 0x05
#define MEMMAP_KERNEL_AND_MODULES     0x06
#define MEMMAP_FRAMEBUFFER            0x07

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
extern const u8 __KernelBinaryDynamicStart;
extern const u8 __KernelBinaryDynamicEnd;
extern const u8 __KernelBinaryBSSStart;
extern const u8 __KernelBinaryBSSEnd;

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
	void FreeBootMemory();
	void InvokeOOM();
}
