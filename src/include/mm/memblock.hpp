#ifndef MM_MEMBLOCK_HPP_
#define MM_MEMBLOCK_HPP_

#include <cstdint.hpp>
#include <sys/list.hpp>
#include <mm/bootmem.hpp>
#include <mm/slab.hpp>

namespace MEM::MEMBLOCK {
	struct MemblockRegion : public ListHead {
		uptr Base;
		usize Length;
		u8 Type;
	};

	struct MemblockAllocator {
		List Regions;

		SLAB::SlabCache *SlabMemblockRegionCache;
		
		void *(*RegionInternalAlloc)(MemblockAllocator *alloc);
	};

	MemblockAllocator *InitializeAllocator();
	void SetupSlabAllocation(MemblockAllocator *alloc, SLAB::SlabAllocator *slab);

	MemblockRegion *AddRegion(MemblockAllocator *alloc, uptr base, usize length, u8 type);

	MemblockRegion *FindRegion(MemblockAllocator *alloc, u8 type, usize size);
	
	inline __attribute__((always_inline))
	MemblockRegion *FindFreeRegion(MemblockAllocator *alloc, usize size) {
		return FindRegion(alloc, MEMMAP_USABLE, size);
	}

	void ListRegions(MemblockAllocator *alloc);
}

#endif /* MM_MEMBLOCK_HPP_ */
