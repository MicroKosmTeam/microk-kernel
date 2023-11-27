#ifndef MM_MEMBLOCK_HPP_
#define MM_MEMBLOCK_HPP_

#include <cstdint.hpp>
#include <sys/list.hpp>
#include <mm/bootmem.hpp>
#include <mm/slab.hpp>

/* For loop to scan all region
	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next);
 */

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

#endif /* MM_MEMBLOCK_HPP_ */
