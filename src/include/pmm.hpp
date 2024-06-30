#pragma once
#include <memory.hpp>
#include <memblock.hpp>

namespace PMM {
	enum PageRequestID {
		ARCH_PAGE_REQUEST,
		BOOTMEM_PAGE_REQUEST,
		DYNAMIC_ALLOCATION_REQUEST, /* This is the last one is usable to infinity,
					       but it's not guaranteed to return positive. */
		TOTAL_PAGE_REQUESTS,
	};

	struct PageRequest {
		uptr Offset;
		usize Length;
	};

	extern PageRequest PageRequests[TOTAL_PAGE_REQUESTS];

	void ValidateMemory();
	void *RequestPage(PageRequestID id);

	void *RequestPage();
	void *RequestPages(usize size);
}
