#include <arch/x64/mm/vmm.hpp>
#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

namespace x86_64 {
VMM::VirtualSpace *NewVirtualSpace() {
	KInfo *info = GetInfo();
	/* We create a new empty page directory */
	PageTable *PML4 = (PageTable*)((uintptr_t)PMM::RequestPage() + info->HigherHalfMapping);
	memset(PML4, 0, PAGE_SIZE);

	VMM::VirtualSpace *space = new PageTableManager(PML4);

	return space;
}

void LoadVirtualSpace(VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();
	/* This loads the page directory into memory */
	PageTable *PML4 = (PageTable*)((uintptr_t)space->GetTopAddress() - info->HigherHalfMapping);

	asm volatile ("mov %0, %%cr3" : : "r" (PML4) : "memory");
}

}
