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

#define MEMMAP_GENERIC_COUNT          8

#define MEMMAP_KERNEL_ESSENTIALS      0x80
#define MEMMAP_KERNEL_TEXT            0x81
#define MEMMAP_KERNEL_RODATA          0x82
#define MEMMAP_KERNEL_DATA            0x83
#define MEMMAP_KERNEL_DYNAMIC         0x84
#define MEMMAP_KERNEL_BSS             0x85
#define MEMMAP_KERNEL_BOOTMEM         0x86
#define MEMMAP_KERNEL_BITMAP          0x87
#define MEMMAP_KERNEL_STACK           0x88
#define MEMMAP_KERNEL_VMALLOC         0x89

#define MEMMAP_KERNEL_SPECIFIC_COUNT  10
#define MEMMAP_KERNEL_SPECIFIC_START  0x80

void *Memset(void *start, u8 value, u64 num);
void *Memclr(void *start, usize num);
void *Memcpy(void *dest, void *src, usize n);
int Memcmp(const void* buf1, const void* buf2, usize count);
void *Memmove(void *dest, const void *src, usize n);

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

	void Init();
	void Deinit();
	const char *MemoryTypeToString(u8 type);
}
