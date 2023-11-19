#pragma once
#include <cstdint.hpp>
#include <sys/list.hpp>

#define SLAB_STATUS_FREE 0
#define SLAB_STATUS_USED 1

namespace MEM::SLAB {
	struct Slab : public ListHead {
		List *ParentList;

		uptr StartAddress;
		usize ActiveElements;
		usize FirstFreeSlot;

		u8 StatusSlots[];
	};

	struct SlabCache {
		List FullSlabs;
		List PartialSlabs;
		List FreeSlabs;

		usize ElementsPerSlab;
		usize ObjectSize;
	};

	void *Alloc(SlabCache *cache);
	void Free(SlabCache *cache, void *ptr);

	SlabCache *InitializeSlabCache(usize objectSize);
	void FreeSlabCache(SlabCache *cache);

	Slab *CreateSlab(SlabCache *cache);
	void FreeSlab(Slab *slab, SlabCache *cache);

	void *ReserveSpaceInSlab(Slab *slab, SlabCache *cache);
	void FreeSpaceInSlab(Slab *slab, SlabCache *cache, void *ptr);
}
