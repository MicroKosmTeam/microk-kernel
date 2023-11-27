#include <mm/bootmem.hpp>
#include <sys/panic.hpp>
#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <cdefs.h>

namespace BOOTMEM {
	struct BootMem {
		usize LastPosition;
		bool Active;

		__attribute__((__aligned__((0x10))))
		u8 Memory[CONFIG_BOOTMEM_SIZE];
	} BootMemData;

	void ActivateBootmem() {
		Memset(BootMemData.Memory, 0, CONFIG_BOOTMEM_SIZE);
		BootMemData.LastPosition = 0;
		BootMemData.Active = true;
	}

	void DeactivateBootmem() {
		BootMemData.Active = false;
	}

	bool BootmemIsActive() {
		return BootMemData.Active;
	}

	void *Malloc(usize size) {
		if (BootMemData.LastPosition + size >= CONFIG_BOOTMEM_SIZE) {
			DeactivateBootmem();
			PANIC("No remaining bootmem memory");
			return NULL;
		}

		void *seg = BootMemData.Memory + BootMemData.LastPosition;
		BootMemData.LastPosition += size;

		return seg;
	}

	void Free(void *address) {
		(void)address;

		DeactivateBootmem();

		PRINTK::PrintK(PRINTK_DEBUG "Bootmem memory was freed at: 0x%x\r\n", address);

		PANIC("Bootmem memory was freed");
		return;
	}

	u32 GetFree() {
		return CONFIG_BOOTMEM_SIZE - BootMemData.LastPosition;
	}

	u32 GetTotal() {
		return CONFIG_BOOTMEM_SIZE;
	}
}
