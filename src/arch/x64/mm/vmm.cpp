#include <arch/x64/mm/vmm.hpp>
#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <init/kinfo.hpp>

#define PAGE_SIZE 0x1000

namespace x86_64 {
VMM::VirtualSpace *NewVirtualSpace() {
	/* We create a new empty page directory */
	PageTable *PML4 = (PageTable*)PMM::RequestPage();
	memset(PML4, 0, PAGE_SIZE);

	VMM::VirtualSpace *space = new PageTableManager(PML4);

	return space;
}

void LoadVirtualSpace(VMM::VirtualSpace *space) {
	/* This loads the page directory into memory */
	PageTable *PML4 = space->GetTopAddress();

	asm volatile ("mov %0, %%cr3" : : "r" (PML4) : "memory");
}

}
