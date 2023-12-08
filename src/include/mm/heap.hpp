#pragma once
#include <mm/slab.hpp>
#include <cstdint.hpp>

namespace MEM::HEAP {
	struct Heap {
		SLAB::SlabAllocator *Allocator;

		SLAB::SlabCache *Size32;
		SLAB::SlabCache *Size64;
		SLAB::SlabCache *Size128;
		SLAB::SlabCache *Size256;
		SLAB::SlabCache *Size512;
		SLAB::SlabCache *Size1k;
		SLAB::SlabCache *Size2k;
		SLAB::SlabCache *Size4k;
		SLAB::SlabCache *Size8k;
		SLAB::SlabCache *Size16k;
	};

	Heap *InitializeHeap(SLAB::SlabAllocator *allocator);

	void *Alloc(Heap *heap, usize size);
	void Free(Heap *heap, void *address);
	void Free(Heap *heap, void *address, usize size);
}
