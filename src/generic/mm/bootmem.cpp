#include <mm/bootmem.hpp>
#include <sys/panic.hpp>
#include <mm/memory.hpp>
#include <cdefs.h>

__attribute__((__aligned__((0x10)))) u8 bootmemMemory[CONFIG_BOOTMEM_SIZE];
u32 lastPosition = 0;
bool bootmemStatus = true;

namespace BOOTMEM {
	void DeactivateBootmem() {
		bootmemStatus = false;
	}

	bool BootmemIsActive() {
		return bootmemStatus;
	}

	void *Malloc(usize size) {
		if (lastPosition + size > CONFIG_BOOTMEM_SIZE) {
			DeactivateBootmem();
			PANIC("No remaining bootmem memory");
			return NULL;
		}

		void *seg = bootmemMemory + lastPosition;
		lastPosition += size;

		Memset(seg, 0, size);

		return seg;
	}

	void Free(void *address) {
		(void)address;
		return; // Bootmem can't free
	}

	u32 GetFree() {
		return CONFIG_BOOTMEM_SIZE - lastPosition;
	}

	u32 GetTotal() {
		return CONFIG_BOOTMEM_SIZE;
	}
}
