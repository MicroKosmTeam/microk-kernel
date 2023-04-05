#include <arch/x64/mm/vmm.hpp>
#include <sys/panic.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <init/kinfo.hpp>

#define PAGE_SIZE 0x1000

bool initialized = false;

PageTable *PML4;

namespace x86_64 {
void InitVMM() {
	if(initialized) return;

	initialized = true;

	KInfo *info = GetInfo();

	PML4 = (PageTable*)PMM::RequestPage();
	memset(PML4, 0, PAGE_SIZE);

	GlobalPageTableManager = new PageTableManager(PML4);

	PRINTK::PrintK("Kernel page table initialized.\r\n");

	for (uint64_t i = 0; i < info->mMapEntryCount; i++) {
		MMapEntry *entry = info->mMap[i];

		uint64_t base = entry->base - (entry->base % 4096);
		uint64_t top = base + entry->length + (entry->length % 4096);

		PRINTK::PrintK("Mapping %s memory area [0x%x - 0x%x]\r\n", entry->type == MEMMAP_KERNEL_AND_MODULES ? "kernel" : "normal", base, top);

		if (entry->type == MEMMAP_KERNEL_AND_MODULES && entry->base == info->kernelPhysicalBase) {
			for (uint64_t t = base; t < top; t += 4096){
				GlobalPageTableManager->MapMemory((void*)info->kernelVirtualBase + t - info->kernelPhysicalBase, (void*)t);
			}
		} else {
			for (uint64_t t = base; t < top; t += 4096){
				GlobalPageTableManager->MapMemory((void*)t, (void*)t);
				GlobalPageTableManager->MapMemory((void*)t + info->higherHalfMapping, (void*)t);
			}
		}
	}


	PRINTK::PrintK("Done mapping.\r\n");

	asm volatile ("mov %0, %%cr3" : : "r" (PML4) : "memory");
}
}
