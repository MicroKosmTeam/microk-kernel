#include <mm/memblock.hpp>

#include <sys/printk.hpp>

namespace MEM::MEMBLOCK {
static void *BootmemRegionInternalAlloc(MemblockAllocator *alloc) {
	(void)alloc;
	return BOOTMEM::Malloc(sizeof(MemblockRegion));
}

static void *NormalRegionInternalAlloc(MemblockAllocator *alloc) {
	return SLAB::Alloc(alloc->SlabMemblockRegionCache);
}

MemblockAllocator *InitializeAllocator() {
	MemblockAllocator *allocator = (MemblockAllocator*)BOOTMEM::Malloc(sizeof(MemblockAllocator));
	allocator->SlabMemblockRegionCache = NULL;
	allocator->RegionInternalAlloc = BootmemRegionInternalAlloc;

	allocator->Regions.Head = NULL;
	allocator->Regions.Tail = NULL;
	
	return allocator;
}
	
void SetupSlabAllocation(MemblockAllocator *alloc, SLAB::SlabAllocator *slab) {
	alloc->SlabMemblockRegionCache = SLAB::InitializeSlabCache(slab, sizeof(MemblockRegion));

	alloc->RegionInternalAlloc = NormalRegionInternalAlloc;
}
	
MemblockRegion *AddRegion(MemblockAllocator *alloc, uptr base, usize length, u8 type) {
	MemblockRegion *region;

	if (alloc->Regions.Head == NULL) {
		region = (MemblockRegion*)alloc->RegionInternalAlloc(alloc);

		region->Base = base;
		region->Length = length;
		region->Type = type;

		alloc->Regions.Head = region;
		alloc->Regions.Tail = region;
		region->Previous = NULL;
		region->Next = NULL;

		return region;
	} else {
		MemblockRegion *current;

		for(current = (MemblockRegion*)alloc->Regions.Head ; current != NULL ; current = (MemblockRegion*)current->Next) {
			if (current->Base > base) {
				if (base + length > current->Base + current->Length) {
					/* Larger region, can't handle */

					return NULL;
				} else if (base + length == current->Base + current->Length) {
					/* Larger region but with same end, could handle */

					return NULL;
				} else /* base + length < current->Base + current->Length) */ {
					/* Insert before */
					region = (MemblockRegion*)alloc->RegionInternalAlloc(alloc);
				
					region->Base = base;
					region->Length = length;
					region->Type = type;
					
					if (current->Previous != NULL) {
						current->Previous->Next = region;
					} else {
						alloc->Regions.Head = region;
					}
						
					region->Previous = current->Previous;
					region->Next = current;

					current->Base += length;
					current->Length -= length;

					current->Previous = region;

					return region;
				}
			} else if (current->Base == base) {
				if (current->Length > length) {
					/* Split at the start */
					region = (MemblockRegion*)alloc->RegionInternalAlloc(alloc);
				
					region->Base = base;
					region->Length = length;
					region->Type = type;
					
					if (current->Previous != NULL) {
						current->Previous->Next = region;
					} else {
						alloc->Regions.Head = region;
					}
						
					region->Previous = current->Previous;
					region->Next = current;

					current->Base += length;
					current->Length -= length;

					current->Previous = region;
				} else if (current->Length == length) {
					/* Same region, just change the type */

					current->Type = type;
					return current;
				} else /* current->Length < length */ {
					/* Larger region, can't handle */

					return NULL;
				}
			} else /* current->Base < base */ {
				if (current->Base + current->Length > base + length) {
					/* Split and insert in the middle */
					region = (MemblockRegion*)alloc->RegionInternalAlloc(alloc);
				
					region->Base = base;
					region->Length = length;
					region->Type = type;
				} else if (current->Base + current->Length == base + length) {
					/* Split at the end */
					region = (MemblockRegion*)alloc->RegionInternalAlloc(alloc);
				
					region->Base = base;
					region->Length = length;
					region->Type = type;
			
					if (current->Next != NULL) {
						current->Next->Previous = region;
					} else {
						alloc->Regions.Tail = region;
					}
						
					region->Next = current->Next;
					region->Previous = current;

					current->Length -= length;

					current->Next = region;

					return region;
				} else /* current->Base + current->Length < base + length */ {
					if (current->Next != NULL) {
						/* Continue until we find a suitable block */
						continue;
					} else {
						/* This is the last block, add it here */
						region = (MemblockRegion*)alloc->RegionInternalAlloc(alloc);

						region->Base = base;
						region->Length = length;
						region->Type = type;

						alloc->Regions.Tail = region;

						region->Next = current->Next;
						region->Previous = current;

						current->Next = region;

						return region;
					}
				}
			}
		}

	}

	return NULL;
}

MemblockRegion *FindRegion(MemblockAllocator *alloc, u8 type, usize size) {
	MemblockRegion *current;

	for(current = (MemblockRegion*)alloc->Regions.Head ; current != NULL ; current = (MemblockRegion*)current->Next) {
		if (current->Length >= size && current->Type == type) {
			return current;
		}
	}

	return NULL;

}
	
usize GetTotalMemorySize(MemblockAllocator *alloc) {
	usize totalSize = 0;
	for (MemblockRegion *current = (MemblockRegion*)alloc->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		totalSize += current->Length;
	}

	return totalSize;
}
	
usize GetTotalSpanningLength(MemblockAllocator *alloc) {
	MemblockRegion *end, *start;
	start = (MemblockRegion*)alloc->Regions.Head;
	end = (MemblockRegion*)alloc->Regions.Tail;

	if (start != NULL && end != NULL) {
		return (end->Base + end->Length - start->Base);
	} else {
		return 0;
	}
}

void ListRegions(MemblockAllocator *alloc) {
	MemblockRegion *current;

	if (alloc->Regions.Head == NULL) {
		PRINTK::PrintK(PRINTK_DEBUG "Memblock empty.\r\n");
	}

	for(current = (MemblockRegion*)alloc->Regions.Head ; current != NULL ; current = (MemblockRegion*)current->Next) {
		PRINTK::PrintK(PRINTK_DEBUG "Region [0x%x - 0x%x].\r\n", current->Base, current->Base + current->Length);
	}

}

}
