#pragma once

#include <cstdint.hpp>
#include <list.hpp>
#include <memory.hpp>

namespace MEM::MEMBLOCK {
	struct MemblockRegion : public ListHead {
		uptr Base;
		usize Length;
		u8 Type;
	};

	struct MemblockAllocator {
		List Regions;
	};

	MemblockAllocator *InitializeAllocator();

	MemblockRegion *AddRegion(MemblockAllocator *alloc, uptr base, usize length, u8 type);

	MemblockRegion *FindRegion(MemblockAllocator *alloc, u8 type, usize size, bool fromHead);

	inline __attribute__((always_inline))
	MemblockRegion *FindFreeRegion(MemblockAllocator *alloc, usize size, bool fromHead) {
		return FindRegion(alloc, MEMMAP_USABLE, size, fromHead);
	}

	void ListRegions(MemblockAllocator *alloc);
	
	usize GetTotalMemorySize(MemblockAllocator *alloc);
	usize GetTotalSpanningLength(MemblockAllocator *alloc);
	usize GetTotalElements(MemblockAllocator *alloc);
}
