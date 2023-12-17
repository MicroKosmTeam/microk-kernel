#include <bootmem.hpp>
#include <panic.hpp>
#include <memory.hpp>
#include <printk.hpp>
#include <vmm.hpp>
#include <memblock.hpp>
#include <cdefs.h>
#include <kinfo.hpp>

namespace BOOTMEM {
static BootMemoryStruct BootMemory;

usize ActivateBootMemory(uptr memoryStart, usize length) {
	if (length > MAXIMUM_BOOTMEM_MEMORY) {
		length = MAXIMUM_BOOTMEM_MEMORY;
	}

	BootMemory.MemoryStart = memoryStart;
	BootMemory.MaximumPosition = length;

	Memclr((void*)memoryStart, length);

	BootMemory.LastPosition = 0;
	BootMemory.Active = true;

	return length;
}

BootMemoryStruct *DeactivateBootMemory() {
	KInfo *info = GetInfo();

	ROUND_UP_TO_PAGE(BootMemory.LastPosition);

	usize usableBootMemory = BootMemory.MaximumPosition - BootMemory.LastPosition;
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks,
			VMM::VirtualToPhysical(BootMemory.MemoryStart) + BootMemory.LastPosition,
			usableBootMemory, MEMMAP_USABLE);

	BootMemory.Active = false;

	return &BootMemory;
}

bool IsBootMemoryActive() {
	return BootMemory.Active;
}

__attribute__((malloc))
void *Malloc(usize size) {
	if (BootMemory.LastPosition + size >= BootMemory.MaximumPosition) {
		DeactivateBootMemory();

		PANIC("No remaining bootmem memory");

		__builtin_unreachable();
	}

	void *seg = (void*)(BootMemory.MemoryStart + BootMemory.LastPosition);
	BootMemory.LastPosition += size;

	return seg;
}
}
