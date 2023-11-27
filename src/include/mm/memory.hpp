#ifndef MM_MEMORY_HPP_
#define MM_MEMORY_HPP_

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
#define MEMMAP_KERNEL_BITMAP          0x86
#define MEMMAP_KERNEL_STACK           0x87
#define MEMMAP_KERNEL_DEVICE          0x88
#define MEMMAP_KERNEL_FILE            0x89

#define MEMMAP_KERNEL_SPECIFIC_COUNT  10
#define MEMMAP_KERNEL_SPECIFIC_START  0x80

void *Memset(void *start, u8 value, u64 num);
void *Memcpy(void *dest, void *src, usize n);
int Memcmp(const void* buf1, const void* buf2, usize count);
void *Memmove(void *dest, const void *src, usize n);

void *Malloc(usize size);

void Free(void *p);
void Free(void *p, usize size);

inline __attribute__((always_inline))
void *operator new(usize size) {
	return Malloc(size);
}

inline __attribute__((always_inline))
void *operator new[](usize size) {
	return Malloc(size);
}

inline __attribute__((always_inline))
void operator delete(void *ptr) {
	Free(ptr);
}

inline __attribute__((always_inline))
void operator delete(void *ptr, usize size) {
	Free(ptr, size);
}

inline __attribute__((always_inline))
void operator delete[](void *ptr) {
	Free(ptr);
}

inline __attribute__((always_inline))
void operator delete[](void *ptr, usize size) {
	Free(ptr, size);
}

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
		if (type >= 0x80) {
			type -= 0x80;

			const char *KernelMemTypeStrings[MEMMAP_KERNEL_SPECIFIC_COUNT] = {
				"Kernel Essentials",
				"Kernel Text",
				"Kernel ROData",
				"Kernel Data",
				"Kernel Dynamic",
				"Kernel BSS",
				"Kernel Bitmap",
				"Kernel Stack",
				"Kernel Device",
				"Kernel File"
			};

			return KernelMemTypeStrings[type];
		}

		const char *GenericMemTypeStrings[MEMMAP_GENERIC_COUNT] = {
			"Usable",
			"Reserved",
			"ACPI Reclaimable",
			"ACPI NVS",
			"Bad",
			"Bootloader Reclaimable",
			"Kernel And Modules",
			"Framebuffer"
		};

		return GenericMemTypeStrings[type];
	}

	void Init();
	void CatalogueKernelMemory();
	void FreeBootMemory();
	void InvokeOOM();
}

#endif /* MM_MEMORY_HPP_ */
