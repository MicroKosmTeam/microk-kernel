#include <arch/x64/mm/vmm.hpp>

namespace x86_64 {

__attribute__((always_inline))
uptr AllocatePage() {
	uptr address = PMM::RequestPage();
	Memset(VMM::PhysicalToVirtual(address), 0 , PAGE_SIZE);
	return address;
}

__attribute__((always_inline))
uptr NewVirtualSpace() {
	/* We create a new empty page directory */
	uptr table = AllocatePage();
	table = VMM::PhysicalToVirtual(table);

	return table;
}

__attribute__((always_inline))
void LoadVirtualSpace(uptr topLevel) {
	/* This loads the page directory into memory */
	asm volatile ("mov %0, %%cr3" : : "r" (VMM::VirtualToPhysical(topLevel)) : "memory");
}

void MapPage(uptr rootPageTable, uptr virt, uptr phys, usize flags) {
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
		table = (volatile u64*) PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff);
	}

	/* 'table' now points to the PTE we're interested in modifying */
	*table = phys | flags;
}
	
uptr FindMappedPage(uptr rootPageTable, uptr virt) {
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
			return -1;
			value = AllocatePage();
			*table = value | PT_Flag::Present | PT_Flag::ReadWrite | PT_Flag::UserSuper;
		}

		--levels;
		table = (volatile u64*) PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff);
	}

	/* 'table' now points to the PTE we're interested in modifying */

	if (*table & (1 << PT_Flag::Present)) {
		return -1;
	}

	uptr address = *table & 0x7ffffffffffff000;

	return address;
}

void UnmapPage(uptr rootPageTable, uptr virt) {
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
			return;
		}

		--levels;
		table = (volatile u64*) PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff);
	}

	/* 'table' now points to the PTE we're interested in modifying */
	*table &= ~(u64)(1 << PT_Flag::Present);
}
}
