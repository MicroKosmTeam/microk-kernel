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

	PRINTK::PrintK(PRINTK_DEBUG "Slab allocator initialized.\r\n");

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
	(void)cache;
	(void)ptr;
}

SlabCache *InitializeSlabCache(SlabAllocator *allocator, usize objectSize) {
	if (objectSize < SLAB_MINIMUM_ALLOC) {
		objectSize = SLAB_MINIMUM_ALLOC;
	}

	SlabCache *cache = (SlabCache*)allocator->SlabCacheInternalAlloc(allocator);
	Memclr(cache, sizeof(SlabCache));

	usize pageCount = objectSize / PAGE_SIZE + 1; /*
	for (usize pages = pageCount; pages <= SLAB_MAXIMUM_PAGES; ++pages) {
		usize waste = pages * PAGE_SIZE % (objectSize + sizeof(u32));

		if (waste <= SLAB_MINIMUM_ALLOC) {
			pageCount = pages;
			break;
		}
	}

	PRINTK::PrintK(PRINTK_DEBUG "%d pages for least waste with object of size %d\r\n", pageCount, objectSize + sizeof(u32));*/

	cache->PagesPerSlab = pageCount;

	cache->Allocator = allocator;
	cache->ObjectSize = objectSize;
	cache->ElementsPerSlab = PAGE_SIZE * cache->PagesPerSlab / (objectSize + sizeof(u32)); /* Object size + signature */
	
	CreateSlab(cache);

	return cache;
}

void FreeSlabCache(SlabCache *cache) {
	(void)cache;
}

Slab *CreateSlab(SlabCache *cache) {
	Slab *slab = (Slab*)cache->Allocator->SlabInternalAlloc(cache->Allocator);
	Memclr(slab, sizeof(Slab));
	
	slab->ParentList = &cache->FreeSlabs;
	AddElementToList(slab, &cache->FreeSlabs);

	if (cache->PagesPerSlab == 1) {
		slab->StartAddress = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	} else {
		slab->StartAddress = VMM::PhysicalToVirtual((uptr)PMM::RequestPages(cache->PagesPerSlab));
	}

	slab->ActiveElements = 0;
	slab->FirstFreeSlot = 0;

	for (usize element = 0; element < cache->ElementsPerSlab; ++element) {
		u32 *slot = (u32*)(slab->StartAddress + (cache->ObjectSize + sizeof(u32)) * element);

		*slot = SLAB_ELEMENT_FREE;
	}

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
			if (slab->ActiveElements + 2 >= cache->PagesPerSlab * PAGE_SIZE / cache->ObjectSize) {
				cache->EmergencyAllocate = true;

				slab = CreateSlab(cache);

				cache->EmergencyAllocate = false;
			}
		}
	}

	u32 *slot = (u32*)(slab->StartAddress + (cache->ObjectSize + sizeof(u32)) * slab->FirstFreeSlot);
	*slot = SLAB_ELEMENT_USED;

	void *addr = (void*)((uptr)slot + sizeof(u32));

	slab->FirstFreeSlot = (usize)-1;

	for (usize element = 0; element < cache->ElementsPerSlab; ++element) {
		u32 *slot = (u32*)(slab->StartAddress + (cache->ObjectSize + sizeof(u32)) * element);

		if (*slot == SLAB_ELEMENT_FREE) {
			slab->FirstFreeSlot = element;
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
	if ((uptr)ptr < slab->StartAddress) {
		return;
	}

	u32 *slot = (u32*)((uptr)ptr - sizeof(u32));

	if (*slot != SLAB_ELEMENT_USED) {
		return;
	}

	usize element = ((uptr)slot - slab->StartAddress) / (cache->ObjectSize + sizeof(u32));
		
	slab->FirstFreeSlot = element;
	*slot = SLAB_STATUS_FREE;
	--slab->ActiveElements;

	if (slab->ParentList == &cache->FullSlabs) {
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->PartialSlabs);
	} else if (slab->ActiveElements == 0) {
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->FreeSlabs);
	}
}
}
