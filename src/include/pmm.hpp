#pragma once
#include <cstdint.hpp>

#include <bitmap.hpp>
#include <memory.hpp>

namespace PMM {
	void InitPageFrameAllocator(usize upperLimit);

	void *RequestPage();
	void *RequestPages(usize pages);

	bool LockPage(void *address);
	void LockPages(void *address, u64 page_count);

	bool FreePage(void *address);
	void FreePages(void *address, u64 page_count);

	u64 GetFreeMem();
	u64 GetUsedMem();
	u64 GetReservedMem();
}
