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
	/* We make sure that it hasn't been initialized more than once */
	if(initialized) return;

	initialized = true;

	KInfo *info = GetInfo();

	/* We create a new empty page directory */
	PML4 = (PageTable*)PMM::RequestPage();
	memset(PML4, 0, PAGE_SIZE);

	GlobalPageTableManager = new PageTableManager(PML4);

	PRINTK::PrintK("Kernel page table initialized.\r\n");

	/* We go through every entry in the memory map and map it in virtual memory */
	for (uint64_t i = 0; i < info->mMapEntryCount; i++) {
		MMapEntry entry = info->mMap[i];

		/* We will skip any memory that is not usable by our kernel, to make the process faster */
		/* We also ignore ACPI, as our kernel is not interested by the information contained in those structures */
		/* It is the responsibility of the modules to manage any ACPI related code, and also to free reclaimable areas */
		if (entry.type == MEMMAP_BAD_MEMORY ||
		    entry.type == MEMMAP_RESERVED ||
		    entry.type == MEMMAP_ACPI_NVS ||
		    entry.type == MEMMAP_ACPI_RECLAIMABLE) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uint64_t base = entry.base - (entry.base % PAGE_SIZE);
		uint64_t top = base + entry.length + (entry.length % PAGE_SIZE);

		/* If it's kernel code, we will map its special location, otherwise we do the lower half and higher half mappings. */
		/* We use the kernel base to be sure we are not mapping module code over the kernel code. */
		if (entry.type == MEMMAP_KERNEL_AND_MODULES && entry.base == info->kernelPhysicalBase) {
			for (uint64_t t = base; t < top; t += PAGE_SIZE){
				GlobalPageTableManager->MapMemory((void*)info->kernelVirtualBase + t - info->kernelPhysicalBase, (void*)t);
			}
		} else {
			for (uint64_t t = base; t < top; t += PAGE_SIZE){
				GlobalPageTableManager->MapMemory((void*)t, (void*)t);
				GlobalPageTableManager->MapMemory((void*)t + info->higherHalfMapping, (void*)t);
			}
		}
	}


	PRINTK::PrintK("Done mapping.\r\n");

	/* This loads the page directory into memory */
	asm volatile ("mov %0, %%cr3" : : "r" (PML4) : "memory");
}
}
