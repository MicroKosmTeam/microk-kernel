#include <mm/heap.hpp>
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
	(void)heap;
	(void)size;

	return NULL;
}

void Free(Heap *heap, void *address) {
	(void)heap;
	(void)address;
}

}

