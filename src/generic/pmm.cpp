#include <pmm.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <kinfo.hpp>

namespace PMM {
PageRequest PageRequests[TOTAL_PAGE_REQUESTS] = {
#if defined(__x86_64__)
	[TSS_STACK_REQUEST] = { 0, DEFAULT_STACK_SIZE },
#elif defined(__aarch64__)
#endif
	[BOOT_DOMAIN_REQUEST] = { 0, PAGE_SIZE },
	[BOOT_SCHEDULER_REQUEST] = { 0, PAGE_SIZE },

	[ROOT_TCB_REQUEST] = { 0, PAGE_SIZE },
	[ROOT_CSPACE_REQUEST] = { 0, PAGE_SIZE },
	[ROOT_CONTEXT_REQUEST] = { 0, PAGE_SIZE },
	[ROOT_VIRTUAL_TOP_REQUEST] = { 0, PAGE_SIZE },
	[ROOT_VIRTUAL_REGISTERS_REQUEST] = { 0, PAGE_SIZE },

	[BOOTMEM_PAGE_REQUEST] = { 0, PAGE_SIZE * 4 },
	[DYNAMIC_ALLOCATION_REQUEST] = { 0, PAGE_SIZE * 0 },
};

void ValidateMemory() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK_DEBUG "Validation of static memory requests:\r\n");

	usize totalAllocationsRequested = 0;
	for (int i = 0; i < TOTAL_PAGE_REQUESTS; ++i) {
		PageRequests[i].Offset = totalAllocationsRequested;
		totalAllocationsRequested += PageRequests[i].Length;
	
		PRINTK::PrintK(PRINTK_DEBUG "#%d -> [0x%x - 0x%x]\r\n",
				i,
				PageRequests[i].Offset,
				PageRequests[i].Offset + PageRequests[i].Length);
	}

	if (info->DesignedAllocationRegionLength < totalAllocationsRequested) {
		PANIC("Insufficient memory");
	}

	PRINTK::PrintK(PRINTK_DEBUG "Total: [0x%x - 0x%x]\r\n"
				    "Used:  [0x%x - 0x%x]\r\n"
				    "Free:  [0x%x - 0x%x]\r\n",
				    info->DesignedAllocationRegionBase,
				    info->DesignedAllocationRegionBase +
				    	info->DesignedAllocationRegionLength,
				    info->DesignedAllocationRegionBase,
				    info->DesignedAllocationRegionBase +
				    	totalAllocationsRequested,
				    info->DesignedAllocationRegionBase +
					totalAllocationsRequested,
				    info->DesignedAllocationRegionBase +
				    	info->DesignedAllocationRegionLength);

				    
}

void *RequestPage(PageRequestID id) {
	KInfo *info = GetInfo();

	if (id >= TOTAL_PAGE_REQUESTS) {
		return NULL;
	}

	PRINTK::PrintK(PRINTK_DEBUG "Page request #%d: [0x%x - 0x%x]\r\n",
				id,
				PageRequests[id].Offset,
				PageRequests[id].Offset + PageRequests[id].Length);

	uptr offset = PageRequests[id].Offset;
	if (id == DYNAMIC_ALLOCATION_REQUEST) {
		if (offset + PageRequests[id].Length > info->DesignedAllocationRegionLength) {
			PANIC("Insufficient memory");
		}

		/* While all other page requests are univocal, the dynamic allocation requests
		 * automatically moves forward the offset when the allocation is completed
		 */
		PageRequests[id].Offset += PageRequests[id].Length;
	}
		
	return (void*)(info->DesignedAllocationRegionBase + offset);
}

void *RequestPage() {
	PageRequests[DYNAMIC_ALLOCATION_REQUEST].Length = PAGE_SIZE;
	return RequestPage(DYNAMIC_ALLOCATION_REQUEST);
}

void *RequestPages(usize size) {
	ROUND_UP_TO_PAGE(size);
	PageRequests[DYNAMIC_ALLOCATION_REQUEST].Length = size;
	return RequestPage(DYNAMIC_ALLOCATION_REQUEST);
}
}
