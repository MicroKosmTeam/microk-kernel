#ifndef MM_SLAB_HPP_
#define MM_SLAB_HPP_

#include <cstdint.hpp>
#include <sys/list.hpp>
#include <mm/vmm.hpp>

#define SLAB_STATUS_FREE 0
#define SLAB_STATUS_USED 1

#define SLAB_MINIMUM_ALLOC 32
#define SLAB_MAXIMUM_PAGES 256 

#define SLAB_ELEMENT_FREE 0xDEAD
#define SLAB_ELEMENT_USED 0xDA7A

namespace MEM::SLAB {
	struct Slab : public ListHead {
		List *ParentList;

		uptr StartAddress;
		usize ActiveElements;
		usize FirstFreeSlot;
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

		usize PagesPerSlab;

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
	bool FreeSpaceInSlab(Slab *slab, SlabCache *cache, void *ptr);
}

#endif /* MM_SLAB_HPP_ */
