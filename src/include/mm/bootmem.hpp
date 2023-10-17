#pragma once
#include <cstdint.hpp>


namespace BOOTMEM {
	void *Malloc(usize size);
	void Free(void *address);

	u32 GetFree();
	u32 GetTotal();

	void DeactivateBootmem();
	bool BootmemIsActive();
}
