#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/mutex.hpp>

static size_t FreeMemory;
static size_t ReservedMemory;
static size_t UsedMemory;
static bool Initialized = false;
static Bitmap PageBitmap;
static size_t PageBitmapIndex = 0; // Last page searched
static uintptr_t HigherHalf;

static void InitBitmap(size_t bitmapSize, void *bufferAddress);
static void UnreservePage(void *address);
static void UnreservePages(void *address, size_t pageCount);
static void ReservePage(void *address);
static void ReservePages(void *address, size_t pageCount);

static void InitBitmap(size_t bitmapSize, void *bufferAddress) {
	PageBitmap.size = bitmapSize;
	PageBitmap.buffer = (uint8_t*)bufferAddress;
	for (size_t i = 0; i < bitmapSize; i++) {
		*(uint8_t*)(PageBitmap.buffer + i) = 0;
	}
}
static void UnreservePage(void *address) {
	size_t index = (size_t)address / 4096;
	if(PageBitmap[index] == false) return;
	if(PageBitmap.Set(index, false)) {
		FreeMemory += 4096;
		ReservedMemory -= 4096;
		if(PageBitmapIndex > index) {
			PageBitmapIndex = index; // Making sure that we don't miss free pages
		}
	}
}

static void ReservePage(void *address) {
	size_t index = (size_t)address / 4096;
	if(PageBitmap[index] == true) return;
	if(PageBitmap.Set(index, true)) {
		FreeMemory -= 4096;
		ReservedMemory += 4096;
	}
}
static void UnreservePages(void *address, size_t pageCount) {
	for (size_t i = 0; i < pageCount; i++) {
		UnreservePage((void*)((size_t)address + (i * 4096)));
	}
}

static void ReservePages(void *address, size_t pageCount) {
	for (size_t i = 0; i < pageCount; i++) {
		ReservePage((void*)((size_t)address + (i * 4096)));
	}
}


namespace PMM {
void InitPageFrameAllocator() {
	if (Initialized) return;

	Initialized = true;

	KInfo *info = GetInfo();

	HigherHalf = info->HigherHalfMapping; 

	void *largestFree = NULL;
	size_t largestFreeSize = 0;

	size_t memorySize = 0;
	for (size_t i = 0; i < info->MemoryMapEntryCount; i++) {
		MEM::MMapEntry desc = info->MemoryMap[i];
		if (desc.type == MEMMAP_USABLE) {
			if(desc.length > largestFreeSize) {
				largestFree = (void*)desc.base;
				largestFreeSize = desc.length;
			}
		}

		if (desc.type != MEMMAP_RESERVED) memorySize += desc.length;
	}

	size_t bitmapSize = memorySize / 4096 / 8 + 1;

	FreeMemory = memorySize;

	largestFree = (void*)((uintptr_t)largestFree + HigherHalf);

	// Initialize bitmap
	InitBitmap(bitmapSize, largestFree);

	// Reserve all pages
	ReservePages(0, memorySize / 4096 + 1);

	// Unreserve usable pages (we do it because the mmap can have holes in it)
	for (size_t i = 0; i < info->MemoryMapEntryCount; i++){
		MEM::MMapEntry desc = info->MemoryMap[i];
		if (desc.type == MEMMAP_USABLE) {
			UnreservePages((void*)desc.base, desc.length / 4096);
		}
	}

	// Locking the memory below 1MB
	ReservePages(0, 0x100);

	// Locking the page bitmap
	ReservePages(PageBitmap.buffer, PageBitmap.size / 4096 + 1);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Bitmap allocator started: %dkb of memory in total.\r\n", memorySize / 1024);
}

#define MAX_TRIES 4 
void *RequestPage() {
	for (int i = 0; i < MAX_TRIES; ++i) {
		for (; PageBitmapIndex < PageBitmap.size * 8; PageBitmapIndex++) {
			if(PageBitmap[PageBitmapIndex] == true) continue;
			LockPage((void*)(PageBitmapIndex * 4096));

			return (void*)(PageBitmapIndex * 4096);
		}

		// Try again 
		PageBitmapIndex = 0;
	}
	
	return NULL;
}

void *RequestPages(size_t pages) {
	for (int i = 0; i < MAX_TRIES; ++i) {
		for (; PageBitmapIndex < (PageBitmap.size - pages)* 8; PageBitmapIndex++) {
			bool free = true;
			for (size_t i = 0; i < pages; i++) {
				if(PageBitmap[PageBitmapIndex + i]) { free = false; break; };
			}

			if (free) {
				LockPages((void*)(PageBitmapIndex * 4096), pages);

				return (void*)(PageBitmapIndex * 4096);
			}

		}
	}

	// No more memory
	return NULL;
}

bool FreePage(void *address) {
	size_t index = (size_t)address / 4096;
	if(PageBitmap[index] == false) {
		return false;
	}

	if(PageBitmap.Set(index, false)) {
		FreeMemory += 4096;
		UsedMemory -= 4096;

		if(PageBitmapIndex > index) {
			PageBitmapIndex = index; // Making sure that we don't miss free pages
		}
	}

	return true;
}

bool LockPage(void *address) {
	size_t index = (size_t)address / 4096;
	if(PageBitmap[index] == true) {
		return false;
	}
	
	if(PageBitmap.Set(index, true)) {
		FreeMemory -= 4096;
		UsedMemory += 4096;
	}

	return true;
}



void FreePages(void *address, size_t pageCount) {
	for (size_t i = 0; i < pageCount; i++) {
		FreePage((void*)((size_t)address + (i * 4096)));
	}
}

void LockPages(void *address, size_t pageCount) {
	for (size_t i = 0; i < pageCount; i++) {
		LockPage((void*)((size_t)address + (i * 4096)));
	}
}

size_t GetFreeMem() {
	return FreeMemory;
}

size_t GetUsedMem() {
	return UsedMemory;
}

size_t GetReservedMem() {
	return ReservedMemory;
}
}
