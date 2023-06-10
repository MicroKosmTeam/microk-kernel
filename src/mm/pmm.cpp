#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/mutex.hpp>

static uint64_t free_memory;
static uint64_t reserved_memory;
static uint64_t used_memory;
static bool initialized = false;
static Bitmap page_bitmap;
static uint64_t page_bitmap_index = 0; // Last page searched
static SpinLock BitmapLock;
static uintptr_t HigherHalf;

static void InitBitmap(size_t bitmap_size, void *buffer_address);
static void UnreservePage(void *address);
static void UnreservePages(void *address, uint64_t page_count);
static void ReservePage(void *address);
static void ReservePages(void *address, uint64_t page_count);

static void InitBitmap(size_t bitmap_size, void *buffer_address) {
	page_bitmap.size = bitmap_size;
	page_bitmap.buffer = (uint8_t*)buffer_address;
	for (int i = 0; i < bitmap_size; i++) {
		*(uint8_t*)(page_bitmap.buffer + i) = 0;
	}
}
static void UnreservePage(void *address) {
	uint64_t index = (uint64_t)address / 4096;
	if(page_bitmap[index] == false) return;
	if(page_bitmap.Set(index, false)) {
		free_memory += 4096;
		reserved_memory -= 4096;
		if(page_bitmap_index > index) {
			page_bitmap_index = index; // Making sure that we don't miss free pages
		}
	}
}

static void ReservePage(void *address) {
	uint64_t index = (uint64_t)address / 4096;
	if(page_bitmap[index] == true) return;
	if(page_bitmap.Set(index, true)) {
		free_memory -= 4096;
		reserved_memory += 4096;
	}
}
static void UnreservePages(void *address, uint64_t page_count) {
	for (int i = 0; i < page_count; i++) {
		UnreservePage((void*)(uint64_t)address + (i * 4096));
	}
}

static void ReservePages(void *address, uint64_t page_count) {
	for (int i = 0; i < page_count; i++) {
		ReservePage((void*)(uint64_t)address + (i * 4096));
	}
}


namespace PMM {
void InitPageFrameAllocator() {
	if (initialized) return;

	initialized = true;

	KInfo *info = GetInfo();

	HigherHalf = info->higherHalfMapping; 

	void *largestFree = NULL;
	size_t largestFreeSize = 0;

	uint64_t memory_size = 0;
	for (int i = 0; i < info->mMapEntryCount; i++) {
		MMapEntry desc = info->mMap[i];
		if (desc.type == MEMMAP_USABLE) {
			if(desc.length > largestFreeSize) {
				largestFree = (void*)desc.base;
				largestFreeSize = desc.length;
			}
		}

		if (desc.type != MEMMAP_RESERVED) memory_size += desc.length;
	}

	uint64_t bitmap_size = memory_size / 4096 / 8 + 1;

	free_memory = memory_size;

	largestFree += HigherHalf;

	// Initialize bitmap
	InitBitmap(bitmap_size, largestFree);

	// Reserve all pages
	ReservePages(0, memory_size / 4096 + 1);

	// Unreserve usable pages (we do it because the mmap can have holes in it)
	for (int i = 0; i < info->mMapEntryCount; i++){
		MMapEntry desc = info->mMap[i];
		if (desc.type == MEMMAP_USABLE) {
			UnreservePages((void*)desc.base, desc.length / 4096);

		}
	}

	// Locking the memory below 1MB
	ReservePages(0, 0x100);

	// Locking the page bitmap
	ReservePages(page_bitmap.buffer, page_bitmap.size / 4096 + 1);

	BitmapLock.Unlock();
	
	PRINTK::PrintK("Bitmap allocator started: %dkb free out of %dkb\r\n", free_memory / 1024, memory_size / 1024);
}

#define MAX_TRIES 4 
void *RequestPage() {
	for (int i = 0; i < MAX_TRIES; ++i) {
		BitmapLock.Lock();

		for (; page_bitmap_index < page_bitmap.size * 8; page_bitmap_index++) {
			if(page_bitmap[page_bitmap_index] == true) continue;
			LockPage((void*)(page_bitmap_index * 4096));

			BitmapLock.Unlock();
			return (void*)(page_bitmap_index * 4096);
		}

		// Try again 
		page_bitmap_index = 0;
		BitmapLock.Unlock();
	}
	
	return NULL;
}

void *RequestPages(size_t pages) {
	for (int i = 0; i < MAX_TRIES; ++i) {
		BitmapLock.Lock();

		for (; page_bitmap_index < (page_bitmap.size - pages)* 8; page_bitmap_index++) {
			bool free = true;
			for (size_t i; i < pages; i++) {
				if(page_bitmap[page_bitmap_index + i]) { free = false; break; };
			}

			if (free) {
				LockPages((void*)(page_bitmap_index * 4096), pages);
				BitmapLock.Unlock();

				return (void*)(page_bitmap_index * 4096);
			}

		}

		BitmapLock.Unlock();
	}

	// No more memory
	return NULL;
}

bool FreePage(void *address) {
//	BitmapLock.Lock();

	uint64_t index = (uint64_t)address / 4096;
	if(page_bitmap[index] == false) {
		BitmapLock.Unlock();
		return false;
	}

	if(page_bitmap.Set(index, false)) {
		free_memory += 4096;
		used_memory -= 4096;

		if(page_bitmap_index > index) {
			page_bitmap_index = index; // Making sure that we don't miss free pages
		}
	}

//	BitmapLock.Unlock();
	return true;
}

bool LockPage(void *address) {
//	BitmapLock.Lock();	

	uint64_t index = (uint64_t)address / 4096;
	if(page_bitmap[index] == true) {
		BitmapLock.Unlock();
		return false;
	}
	
	if(page_bitmap.Set(index, true)) {
		free_memory -= 4096;
		used_memory += 4096;
	}

//	BitmapLock.Unlock();
	return true;
}



void FreePages(void *address, uint64_t page_count) {
	for (int i = 0; i < page_count; i++) {
		FreePage((void*)(uint64_t)address + (i * 4096));
	}
}

void LockPages(void *address, uint64_t page_count) {
	for (int i = 0; i < page_count; i++) {
		LockPage((void*)(uint64_t)address + (i * 4096));
	}
}

uint64_t GetFreeMem() {
	return free_memory;
}

uint64_t GetUsedMem() {
	return used_memory;
}

uint64_t GetReservedMem() {
	return reserved_memory;
}
}
