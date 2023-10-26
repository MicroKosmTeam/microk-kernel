#include <arch/x64/mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>

#include <sys/printk.hpp>

namespace x86_64 {
uptr AllocatePage() {
	uptr address = (uptr)PMM::RequestPage();
	Memset((void*)VMM::PhysicalToVirtual(address), 0 , PAGE_SIZE);
	return address;
}


uptr NewVirtualSpace() {
	/* We create a new empty page directory */
	uptr table = AllocatePage();
	table = VMM::PhysicalToVirtual(table);

	return table;
}


void LoadVirtualSpace(uptr topLevel) {
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "!! Loading 0x%x -> 0x%x\r\n", topLevel, VMM::VirtualToPhysical(topLevel));
	/* This loads the page directory into memory */
	asm volatile ("mov %0, %%cr3" : : "r" (VMM::VirtualToPhysical(topLevel)) : "memory");
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "!! Loaded 0x%x -> 0x%x\r\n", topLevel, VMM::VirtualToPhysical(topLevel));
}

void MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags) {
	/* how many page tables to traverse before we get to the PML1 */
	const bool use5LevelPaging = false;
	int levels = use5LevelPaging ? 4 : 3;

	/* points to the PML5e or PML4e that manages 'virt' */
	volatile u64 *table = (volatile u64*)(rootPageTable + ((virt >> (12 + 9 * levels)) & 0x1ff));

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
		table = (volatile u64*)(VMM::PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff));
	}

	/* 'table' now points to the PTE we're interested in modifying */
	*table = phys | flags;
}
	
uptr FindMappedPage(uptr rootPageTable, uptr virt) {
	/* how many page tables to traverse before we get to the PML1 */
	const bool use5LevelPaging = false;
	int levels = use5LevelPaging ? 4 : 3;

	/* points to the PML5e or PML4e that manages 'virt' */
	volatile u64 *table = (volatile u64*)(rootPageTable + ((virt >> (12 + 9 * levels)) & 0x1ff));

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
		}

		--levels;
		table = (volatile u64*)(VMM::PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff));
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
	volatile u64 *table = (volatile u64*)(rootPageTable + ((virt >> (12 + 9 * levels)) & 0x1ff));

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
		table = (volatile u64*)(VMM::PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff));
	}

	/* 'table' now points to the PTE we're interested in modifying */
	*table &= ~(u64)(1 << PT_Flag::Present);
}
}
