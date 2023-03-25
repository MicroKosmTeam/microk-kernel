#pragma once
#include <stdint.h>
#include <stddef.h>
#include <sys/bitmap.hpp>
#include <mm/memory.hpp>

namespace PMM {
	void InitPageFrameAllocator();

	void *RequestPage();
	void *RequestPages(size_t pages);

	void LockPage(void *address);
	void LockPages(void *address, uint64_t page_count);

	void FreePage(void *address);
	void FreePages(void *address, uint64_t page_count);

	uint64_t GetFreeMem();
	uint64_t GetUsedMem();
	uint64_t GetReservedMem();
}
