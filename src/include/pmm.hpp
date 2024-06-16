#pragma once
#include <memory.hpp>
#include <memblock.hpp>

namespace PMM {
	enum PageRequestID {
#if defined(__x86_64__)
		TSS_STACK_REQUEST,
#elif defined(__aarch64__)
#endif
		BOOT_DOMAIN_REQUEST,
		BOOT_SCHEDULER_REQUEST,

		ROOT_TCB_REQUEST,
		ROOT_CSPACE_REQUEST,
		ROOT_CONTEXT_REQUEST,
		ROOT_VIRTUAL_TOP_REQUEST,
		ROOT_VIRTUAL_REGISTERS_REQUEST,

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
