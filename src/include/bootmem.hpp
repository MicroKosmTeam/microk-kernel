#pragma once
#include <cstdint.hpp>

/* Limiting maximum bootmem allocation space */
#define MAXIMUM_BOOTMEM_MEMORY (64 * 1024 * 1024)

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
