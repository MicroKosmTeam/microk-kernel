#include <mm/bitmap.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>

static usize FreeMemory;
static usize ReservedMemory;
static usize UsedMemory;
static bool Initialized = false;
static Bitmap PageBitmap;
static usize PageBitmapIndex = 0; // Last page searched
static uptr HigherHalf;

static void InitBitmap(usize bitmapSize, void *bufferAddress);
static void UnreservePage(void *address);
static void UnreservePages(void *address, usize pageCount);
static void ReservePage(void *address);
static void ReservePages(void *address, usize pageCount);

static void InitBitmap(usize bitmapSize, void *bufferAddress) {
	PageBitmap.Size = bitmapSize;
	PageBitmap.Buffer = (u8*)bufferAddress;
	for (usize i = 0; i < bitmapSize; i++) {
		*(u8*)(PageBitmap.Buffer + i) = 0;
	}
}
static void UnreservePage(void *address) {
	usize index = (usize)address / PAGE_SIZE;
	if(PageBitmap[index] == false) return;
	if(PageBitmap.Set(index, false)) {
		FreeMemory += PAGE_SIZE;
		ReservedMemory -= PAGE_SIZE;
		if(PageBitmapIndex > index) {
			PageBitmapIndex = index; // Making sure that we don't miss free pages
		}
	}
}

static void ReservePage(void *address) {
	usize index = (usize)address / PAGE_SIZE;
	if(PageBitmap[index] == true) return;
	if(PageBitmap.Set(index, true)) {
		FreeMemory -= PAGE_SIZE;
		ReservedMemory += PAGE_SIZE;
	}
}
static void UnreservePages(void *address, usize pageCount) {
	for (usize i = 0; i < pageCount; i++) {
		UnreservePage((void*)((usize)address + (i * PAGE_SIZE)));
	}
}

static __attribute__((unused)) void ReservePages(void *address, usize pageCount) {
	for (usize i = 0; i < pageCount; i++) {
		ReservePage((void*)((usize)address + (i * PAGE_SIZE)));
	}
}


namespace PMM {
void InitPageFrameAllocator() {
	if (Initialized) return;

	Initialized = true;

	KInfo *info = GetInfo();

	HigherHalf = info->HigherHalfMapping; 

	void *largestFree = NULL;

	usize memorySize = MEM::MEMBLOCK::GetTotalSpanningLength(info->PhysicalMemoryChunks);

	usize bitmapSize = memorySize / PAGE_SIZE / 8 + 1;

	FreeMemory = memorySize;

	ROUND_UP_TO_PAGE(bitmapSize);
	MEM::MEMBLOCK::MemblockRegion *baseRegion = MEM::MEMBLOCK::FindFreeRegion(info->PhysicalMemoryChunks, bitmapSize, true);
	baseRegion = MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, baseRegion->Base, bitmapSize, MEMMAP_KERNEL_BITMAP);
	largestFree = (void*)VMM::PhysicalToVirtual(baseRegion->Base);

	// Initialize bitmap
	InitBitmap(bitmapSize, largestFree);

	// Reserve all pages
	Memset64(largestFree, ~((u64)0), bitmapSize);

	// Unreserve usable pages (we do it because the mmap can have holes in it)
	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		if (current->Type == MEMMAP_USABLE) {
			UnreservePages((void*)current->Base, current->Length / PAGE_SIZE);
		}
	}

	PRINTK::PrintK(PRINTK_DEBUG "Bitmap allocator started: %dkb of memory in total.\r\n", memorySize / 1024);
}

#define MAX_TRIES 2
void *RequestPage() {
	for (int i = 0; i < MAX_TRIES; ++i) {
		for (; PageBitmapIndex < PageBitmap.Size * 8; ++PageBitmapIndex) {
			if(PageBitmap[PageBitmapIndex] == true) continue;
			LockPage((void*)(PageBitmapIndex * PAGE_SIZE));

			return (void*)(PageBitmapIndex * PAGE_SIZE);
		}

		// Try again 
		PageBitmapIndex = 0;
	}

	return NULL;
}

void *RequestPages(usize pages) {
	for (int i = 0; i < MAX_TRIES; ++i) {
		for (; PageBitmapIndex < (PageBitmap.Size - pages)* 8; PageBitmapIndex++) {
			bool free = true;
			for (usize i = 0; i < pages; i++) {
				if(PageBitmap[PageBitmapIndex + i]) { free = false; break; };
			}

			if (free) {
				LockPages((void*)(PageBitmapIndex * PAGE_SIZE), pages);

				return (void*)(PageBitmapIndex * PAGE_SIZE);
			}

		}
	}

	// No more memory
	return NULL;
}

bool FreePage(void *address) {
	usize index = (usize)address / PAGE_SIZE;
	if(PageBitmap[index] == false) {
		return false;
	}

	if(PageBitmap.Set(index, false)) {
		FreeMemory += PAGE_SIZE;
		UsedMemory -= PAGE_SIZE;

		if(PageBitmapIndex > index) {
			PageBitmapIndex = index; // Making sure that we don't miss free pages
		}
	}

	return true;
}

bool LockPage(void *address) {
	usize index = (usize)address / PAGE_SIZE;
	if(PageBitmap[index] == true) {
		return false;
	}
	
	if(PageBitmap.Set(index, true)) {
		FreeMemory -= PAGE_SIZE;
		UsedMemory += PAGE_SIZE;
	}

	return true;
}



void FreePages(void *address, usize pageCount) {
	for (usize i = 0; i < pageCount; i++) {
		FreePage((void*)((usize)address + (i * PAGE_SIZE)));
	}
}

void LockPages(void *address, usize pageCount) {
	for (usize i = 0; i < pageCount; i++) {
		LockPage((void*)((usize)address + (i * PAGE_SIZE)));
	}
}

usize GetFreeMem() {
	return FreeMemory;
}

usize GetUsedMem() {
	return UsedMemory;
}

usize GetReservedMem() {
	return ReservedMemory;
}
}
