#pragma once
#include <cdefs.h>

namespace BOOTMEM {
	struct BootMemoryStruct {
		uptr MemoryStart;
		usize LastPosition;
		usize MaximumPosition;
		bool Active;
	};

	usize ActivateBootMemory(uptr memoryStart, usize length);
	BootMemoryStruct *DeactivateBootMemory();
	bool IsBootMemoryActive();

	__attribute__((malloc)) void *Malloc(usize size);
}
