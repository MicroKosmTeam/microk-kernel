#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mm/bootmem.hpp>
#include <mm/heap.hpp>
#include <init/kinfo.hpp>

void *Malloc(usize size) {
	if(HEAP::IsHeapActive()) return HEAP::Malloc(size);
	else return NULL;
}

void Free(void *p) {
	HEAP::Free(p);
}

void *operator new(usize size) {
	if(HEAP::IsHeapActive()) return HEAP::Malloc(size);
	else return NULL;
}

void *operator new[](usize size) {
	if(HEAP::IsHeapActive()) return HEAP::Malloc(size);
	else return NULL;
}

void operator delete(void* p) {
	if(HEAP::IsHeapActive()) HEAP::Free(p);
}

void operator delete(void* p, usize size) {
	(void)size;
	if(HEAP::IsHeapActive()) HEAP::Free(p);
}

void operator delete[](void* p) {
	if(HEAP::IsHeapActive()) HEAP::Free(p);
}

void operator delete[](void* p, usize size) {
	(void)size;
	if(HEAP::IsHeapActive()) HEAP::Free(p);
}

namespace MEM {
void InvokeOOM() {
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "OOM invoked, system is out of memory.\r\n");

	PANIC("Out of memory");
}

void Init() {
	char *memTypeStrings[] = {
		"Usable",
		"Reserved",
		"ACPI Reclaimable",
		"ACPI NVS",
		"Bad",
		"Bootloader reclaimable",
		"Kernel and modules",
		"Framebuffer"
	};


	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Provided physical RAM map:\r\n");

	for (usize i = 0; i < info->MemoryMapEntryCount; i++) {
		PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " [0x%x - 0x%x] -> %s\r\n",
				info->MemoryMap[i].AddressBase,
				info->MemoryMap[i].AddressBase + info->MemoryMap[i].Length,
				memTypeStrings[info->MemoryMap[i].Type]);
	}

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Contiguous regions:\r\n");
	bool isAllUnusable = true;
	uptr base = info->MemoryMap[0].AddressBase;
	usize length = info->MemoryMap[0].Length;

	for (usize region = 1; region < info->MemoryMapEntryCount; region++) {
		if (info->MemoryMap[region - 1].AddressBase + info->MemoryMap[region - 1].Length == info->MemoryMap[region].AddressBase) {
			length += info->MemoryMap[region].Length;
			if(isAllUnusable) isAllUnusable = info->MemoryMap[region].Type == MEMMAP_USABLE ? false : true;
		} else {
			PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " %s area: [0x%x - 0x%x]\r\n",
				isAllUnusable ? "Unusable" : "Usable",
				base,
				base + length);

			base = info->MemoryMap[region].AddressBase;
			length = info->MemoryMap[region].Length;
			isAllUnusable = info->MemoryMap[region].Type == MEMMAP_USABLE ? false : true;
		}
	}

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " %s area: [0x%x - 0x%x]\r\n",
				isAllUnusable ? "Unusable" : "Usable",
				base,
				base + length);

	/* Enabling the page frame allocator */
	PMM::InitPageFrameAllocator();

	VMM::InitVMM();

	/* Initializing the heap */
	HEAP::InitializeHeap((void*)CONFIG_HEAP_BASE, CONFIG_HEAP_SIZE / PAGE_SIZE);

	/* With the heap initialized, disable new bootmem allocations */
	BOOTMEM::DeactivateBootmem();
}
}
