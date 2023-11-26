#ifndef MM_SLAB_HPP_
#define MM_SLAB_HPP_

#include <cstdint.hpp>
#include <sys/list.hpp>
#include <mm/vmm.hpp>

#define SLAB_STATUS_FREE 0
#define SLAB_STATUS_USED 1

#define SLAB_MINIMUM_ALLOC 32

namespace MEM::SLAB {
	struct Slab : public ListHead {
		List *ParentList;

		uptr StartAddress;
		usize ActiveElements;
		usize FirstFreeSlot;
	
		u8 StatusSlots[PAGE_SIZE / SLAB_MINIMUM_ALLOC];
	};
	
	struct SlabAllocator;

	struct SlabCache {
		SlabAllocator *Allocator;

		List FullSlabs;
		List PartialSlabs;
		List FreeSlabs;

		usize ElementsPerSlab;
		usize ObjectSize;
		usize TotalSlabs;

		bool EmergencyAllocate;
	};

	struct SlabAllocator {
		SlabCache *SlabStructureCache;
		SlabCache *SlabCacheStructureCache;

		void *(*SlabInternalAlloc)(SlabAllocator *alloc);
		void *(*SlabCacheInternalAlloc)(SlabAllocator *alloc);
	};

	SlabAllocator *InitializeAllocator();

	void *Alloc(SlabCache *cache);
	void Free(SlabCache *cache, void *ptr);

	SlabCache *InitializeSlabCache(SlabAllocator *allocator, usize objectSize);
	void FreeSlabCache(SlabCache *cache);

	Slab *CreateSlab(SlabCache *cache);
	void FreeSlab(Slab *slab, SlabCache *cache);

	void *ReserveSpaceInSlab(Slab *slab, SlabCache *cache);
	void FreeSpaceInSlab(Slab *slab, SlabCache *cache, void *ptr);
}

#endif /* MM_SLAB_HPP_ */
