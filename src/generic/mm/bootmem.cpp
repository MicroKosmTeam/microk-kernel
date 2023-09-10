#include <mm/bootmem.hpp>
#include <sys/panic.hpp>
#include <cdefs.h>

__attribute__((__aligned__((16)))) uint8_t bootmemMemory[CONFIG_BOOTMEM_SIZE];
uint32_t lastPosition = 0;
bool bootmemStatus = true;

namespace BOOTMEM {
	void DeactivateBootmem() {
		bootmemStatus = false;
	}

	bool BootmemIsActive() {
		return bootmemStatus;
	}

	void *Malloc(size_t size) {
		if (lastPosition + size > CONFIG_BOOTMEM_SIZE) {
			DeactivateBootmem();
			PANIC("No remaining bootmem memory");
			return NULL;
		}

		void *seg = bootmemMemory + lastPosition;
		lastPosition += size;

		return seg;
	}

	void Free(void *address) {
		(void)address;
		return; // Bootmem can't free
	}

	uint32_t GetFree() {
		return CONFIG_BOOTMEM_SIZE - lastPosition;
	}

	uint32_t GetTotal() {
		return CONFIG_BOOTMEM_SIZE;
	}
}
