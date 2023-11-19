#include <mm/slab.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>

#include <sys/printk.hpp>

namespace MEM::SLAB {
void *Alloc(SlabCache *cache) {
	Slab *slab = (Slab*)GetListHead(&cache->PartialSlabs);
	if (slab == NULL) {
		slab = (Slab*)GetListHead(&cache->FreeSlabs);

		if (slab == NULL) {
			/* Allocate a new slab */
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

SlabCache *InitializeSlabCache(usize objectSize) {
	SlabCache *cache = new SlabCache;
	Memset(cache, 0, sizeof(SlabCache));

	cache->ObjectSize = objectSize;
	cache->ElementsPerSlab = PAGE_SIZE / objectSize;

	CreateSlab(cache);

	return cache;
}

void FreeSlabCache(SlabCache *cache) {
	(void)cache;
}

Slab *CreateSlab(SlabCache *cache) {
	usize structureSize = sizeof(Slab) + cache->ElementsPerSlab * sizeof(u8);
	Slab *slab = (Slab*)Malloc(structureSize);
	Memset(slab, 0, structureSize);

	slab->ParentList = &cache->FreeSlabs;
	AddElementToList(slab, &cache->FreeSlabs);

	slab->StartAddress = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	slab->ActiveElements = 0;
	slab->FirstFreeSlot = 0;

	return slab;
}

void FreeSlab(Slab *slab, SlabCache *cache) {
	(void)slab;
	(void)cache;
}

void *ReserveSpaceInSlab(Slab *slab, SlabCache *cache) {
	slab->StatusSlots[slab->FirstFreeSlot] = SLAB_STATUS_USED;
		
	void *addr = (void*)(slab->StartAddress + slab->FirstFreeSlot * cache->ObjectSize);

	slab->FirstFreeSlot = (usize)-1;
	for (usize slot = 0; slot < cache->ElementsPerSlab; ++slot) {
		if (slab->StatusSlots[slot] == SLAB_STATUS_FREE) {
			slab->FirstFreeSlot = slot;
			break;
		}
	}

	if (slab->FirstFreeSlot == (usize)-1) {
		/* If the slab is full */
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->FullSlabs);
	} else if (slab->ActiveElements == 0) {
		MoveElementToList((ListHead*)slab, slab->ParentList, &cache->PartialSlabs);
	}


	++slab->ActiveElements;

	return addr;
}

void FreeSpaceInSlab(Slab *slab, SlabCache *cache, void *ptr) {
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
