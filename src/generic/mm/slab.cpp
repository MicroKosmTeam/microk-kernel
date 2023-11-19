#include <mm/slab.hpp>
#include <mm/pmm.hpp>
#include <mm/bootmem.hpp>

#include <sys/printk.hpp>

namespace MEM::SLAB {
static void *BootmemSlabInternalAlloc(SlabAllocator *alloc) {
	(void)alloc;
	return BOOTMEM::Malloc(sizeof(Slab));
}

static void *BootmemSlabCacheInternalAlloc(SlabAllocator *alloc) {
	(void)alloc;
	return BOOTMEM::Malloc(sizeof(SlabCache));
}

static void *NormalSlabInternalAlloc(SlabAllocator *alloc) {
	return Alloc(alloc->SlabStructureCache);
}

static void *NormalSlabCacheInternalAlloc(SlabAllocator *alloc) {
	return Alloc(alloc->SlabCacheStructureCache);
}

SlabAllocator *InitializeAllocator() {
	SlabAllocator *allocator = (SlabAllocator*)BOOTMEM::Malloc(sizeof(SlabAllocator));

	allocator->SlabInternalAlloc = BootmemSlabInternalAlloc;
	allocator->SlabCacheInternalAlloc = BootmemSlabCacheInternalAlloc;

	allocator->SlabStructureCache = InitializeSlabCache(allocator, sizeof(Slab));
	allocator->SlabCacheStructureCache = InitializeSlabCache(allocator, sizeof(SlabCache));

	allocator->SlabInternalAlloc = NormalSlabInternalAlloc;
	allocator->SlabCacheInternalAlloc = NormalSlabCacheInternalAlloc;

	return allocator;
}

void *Alloc(SlabCache *cache) {
	Slab *slab = (Slab*)GetListHead(&cache->PartialSlabs);
	if (slab == NULL) {
		slab = (Slab*)GetListHead(&cache->FreeSlabs);

		if (slab == NULL) {
			slab = CreateSlab(cache);
		}
	}
	
	void *addr = ReserveSpaceInSlab(slab, cache);
	return addr;
}

void Free(SlabCache *cache, void *ptr) {
	Slab *slab = NULL;
	List *list = &cache->FullSlabs;
	bool passedFull = false, passedPart = false;

	do {
		if (list->Head != NULL) {
			slab = (Slab*)list->Head;

			while (true) {
				if ((uptr)ptr >= slab->StartAddress &&
				    (uptr)ptr <= slab->StartAddress + PAGE_SIZE) {
					FreeSpaceInSlab(slab, cache, ptr);
					return;
				}

				if (slab == list->Tail) {
					break;
				}

				slab = (Slab*)slab->Next;
			}
		}

		if (list == &cache->FullSlabs) {
			passedFull = true;
			list = &cache->PartialSlabs;
		} else {
			passedPart = true;
		}
	} while(!passedFull || ! passedPart);
}

SlabCache *InitializeSlabCache(SlabAllocator *allocator, usize objectSize) {
	if (objectSize < SLAB_MINIMUM_ALLOC) {
		objectSize = SLAB_MINIMUM_ALLOC;
	}

	SlabCache *cache = (SlabCache*)allocator->SlabCacheInternalAlloc(allocator);
	Memset(cache, 0, sizeof(SlabCache));

	cache->Allocator = allocator;
	cache->ObjectSize = objectSize;
	cache->ElementsPerSlab = PAGE_SIZE / objectSize;

	CreateSlab(cache);

	return cache;
}

void FreeSlabCache(SlabCache *cache) {
	(void)cache;
}

Slab *CreateSlab(SlabCache *cache) {
	Slab *slab = (Slab*)cache->Allocator->SlabInternalAlloc(cache->Allocator);
	Memset(slab, 0, sizeof(Slab));

	slab->ParentList = &cache->FreeSlabs;
	AddElementToList(slab, &cache->FreeSlabs);

	slab->StartAddress = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	slab->ActiveElements = 0;
	slab->FirstFreeSlot = 0;

	++cache->TotalSlabs;

	return slab;
}

void FreeSlab(Slab *slab, SlabCache *cache) {
	(void)slab;
	(void)cache;
}

void *ReserveSpaceInSlab(Slab *slab, SlabCache *cache) {
	if (!cache->EmergencyAllocate) {
		if (cache == cache->Allocator->SlabStructureCache ||
		    cache == cache->Allocator->SlabCacheStructureCache) {
			if (slab->ActiveElements + 2 >= PAGE_SIZE / cache->ObjectSize) {
				cache->EmergencyAllocate = true;

				slab = CreateSlab(cache);

				cache->EmergencyAllocate = false;
			}
		}
	}

	slab->StatusSlots[slab->FirstFreeSlot] = SLAB_STATUS_USED;
		
	void *addr = (void*)(slab->StartAddress + slab->FirstFreeSlot * cache->ObjectSize);

	slab->FirstFreeSlot = (usize)-1;
	for (usize slot = 0; slot < cache->ElementsPerSlab; ++slot) {
		if (slab->StatusSlots[slot] == SLAB_STATUS_FREE) {
			slab->FirstFreeSlot = slot;
			break;
		}
	}
	
	++slab->ActiveElements;

	if (slab->FirstFreeSlot == (usize)-1 || slab->ActiveElements > PAGE_SIZE * cache->ObjectSize) {
		/* If the slab is full */
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->FullSlabs);
	} else if (slab->ActiveElements == 0) {
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->PartialSlabs);
	}

	return addr;
}

void FreeSpaceInSlab(Slab *slab, SlabCache *cache, void *ptr) {
	if ((uptr)ptr % SLAB_MINIMUM_ALLOC) {
		/* Unaligned free */
		return;
	}

	usize element = ((uptr)ptr - slab->StartAddress) / cache->ElementsPerSlab;

	if (slab->StatusSlots[element] == SLAB_STATUS_FREE) {
		return;
	}

	slab->StatusSlots[element] = SLAB_STATUS_FREE;
	--slab->ActiveElements;

	if (slab->ParentList == &cache->FullSlabs) {
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->PartialSlabs);
	} else if (slab->ActiveElements == 0) {
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->FreeSlabs);
	}
}
}
