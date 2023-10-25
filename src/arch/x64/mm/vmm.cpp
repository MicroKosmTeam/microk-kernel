#include <arch/x64/mm/vmm.hpp>
#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

inline uptr AllocatePage() {
	uptr address = PMM::RequestPage();
	Memset(VMM::PhysicalToVirtual(address), 0 , PAGE_SIZE);
	return address;
}

namespace x86_64 {
void MapPage(uptr rootPageTable, uptr virt, uptr phys, u64 flags) {
	/* how many page tables to traverse before we get to the PML1 */
	const bool use5LevelPaging = false;
	int levels = use5LevelPaging ? 4 : 3;

	/* points to the PML5e or PML4e that manages 'virt' */
	volatile u64 *table = rootPageTable + ((virt >> (12 + 9 * levels)) & 0x1ff);

	/*
	 * In a loop, reduce the scope of 'table' by traversing downward and
	 * allocating new page tables as neccessary.
	 */
	while(levels) {
		u64 value = *table;

		if(value & PT_Flag::Present) {
			/* page table already exists */
			value &= 0x7ffffffffffff000;
		} else {
			/* allocate a new page table */
			value = AllocatePage();
			*table = value | PT_Flag::Present | PT_Flag::ReadWrite | PT_Flag::UserSuper;
		}

		--levels;
		table = (volatile u64 *) PhysicalToVirtual(value) + ((virt >> (12 + 9 * l)) & 0x1ff);
	}

	/* 'table' now points to the PTE we're interested in modifying */
	*table = phys | flags;
}

}
