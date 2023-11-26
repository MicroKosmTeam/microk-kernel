#include <mm/heap.hpp>
#include <sys/math.hpp>
#include <mm/bootmem.hpp>
#include <sys/printk.hpp>

namespace MEM::HEAP {
Heap *InitializeHeap(SLAB::SlabAllocator *allocator) {
	Heap *heap = (Heap*)BOOTMEM::Malloc(sizeof(Heap));

	heap->Allocator = allocator;
	heap->Size32 = SLAB::InitializeSlabCache(allocator, 32);
	heap->Size64 = SLAB::InitializeSlabCache(allocator, 64);
	heap->Size128 = SLAB::InitializeSlabCache(allocator, 128);
	heap->Size256 = SLAB::InitializeSlabCache(allocator, 256);
	heap->Size512 = SLAB::InitializeSlabCache(allocator, 512);
	heap->Size1k = SLAB::InitializeSlabCache(allocator, 1024);
	heap->Size2k = SLAB::InitializeSlabCache(allocator, 2048);
	heap->Size4k = SLAB::InitializeSlabCache(allocator, 4096);
	
	PRINTK::PrintK(PRINTK_DEBUG "Kernel heap initialized.\r\n");

	return heap;
}

void *Alloc(Heap *heap, usize size) {
	if (size < 32) {
		size = 32;
	}

	if (size > 4096) {
		return NULL;
	}

	size = MATH::UpperPowerOfTwoUSIZE(size);
	
	switch (size) {
		case 32:
			return SLAB::Alloc(heap->Size32);
		case 64:
			return SLAB::Alloc(heap->Size64);
		case 128:
			return SLAB::Alloc(heap->Size128);
		case 256:
			return SLAB::Alloc(heap->Size256);
		case 512:
			return SLAB::Alloc(heap->Size512);
		case 1024:
			return SLAB::Alloc(heap->Size1k);
		case 2048:
			return SLAB::Alloc(heap->Size2k);
		case 4096:
			return SLAB::Alloc(heap->Size4k);
		default:
			return NULL;
	}
}

void Free(Heap *heap, void *address, usize size) {
	if (size < 32) {
		size = 32;
	}

	if (size > 4096) {
		return;
	}

	size = MATH::UpperPowerOfTwoUSIZE(size);

	switch (size) {
		case 32:
			return SLAB::Free(heap->Size32, address);
		case 64:
			return SLAB::Free(heap->Size64, address);
		case 128:
			return SLAB::Free(heap->Size128, address);
		case 256:
			return SLAB::Free(heap->Size256, address);
		case 512:
			return SLAB::Free(heap->Size512, address);
		case 1024:
			return SLAB::Free(heap->Size1k, address);
		case 2048:
			return SLAB::Free(heap->Size2k, address);
		case 4096:
			return SLAB::Free(heap->Size4k, address);
		default:
			return;
	}
}

void Free(Heap *heap, void *address) {
	(void)heap;
	(void)address;
}

}

