#pragma once
#include <memory.hpp>
#include <memblock.hpp>

namespace PMM {
	struct PhysicalMemoryManagerStruct {
		MEM::MEMBLOCK::MemblockRegion *FreeRegion;
		MEM::MEMBLOCK::MemblockRegion *VMAllocRegion;

		bool IsActive;
	};

	void Init();
	void Deinit();

	void *RequestPage();
	void *RequestPages(usize length);
}
