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
	/* This loads the page directory into memory */
	asm volatile ("mov %0, %%cr3" : : "r" (VMM::VirtualToPhysical(topLevel)) : "memory");
}

volatile u64 *GetNextLevel(volatile u64 *topLevel, usize idx, bool allocate) {
	if ((topLevel[idx] & (1 << PT_Flag::Present)) != 0) {
		return (volatile u64*)(VMM::PhysicalToVirtual(PTE_GET_ADDR(topLevel[idx])));
	}

	if (!allocate) {
		return NULL;
	}

	uptr newPage = AllocatePage();
	if(newPage == 0) return NULL;

	topLevel[idx] = newPage | (1 << PT_Flag::Present) | (1 << PT_Flag::UserSuper) | (1 << PT_Flag::ReadWrite);
	return (volatile u64*)VMM::PhysicalToVirtual(newPage);
}

bool MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags) {
	bool ok = false;

	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4 = (volatile u64*)rootPageTable;
	volatile u64 *pml3 = GetNextLevel(pml4, pml4Entry, true);
	if (pml3 == NULL) {
		return ok;
	}

	volatile u64 *pml2 = GetNextLevel(pml3, pml3Entry, true);
	if (pml2 == NULL) {
		return ok;
	}

	volatile u64 *pml1 = GetNextLevel(pml2, pml2Entry, true);
	if (pml1 == NULL) {
		return ok;
	}

	if ((pml1[pml1Entry] & (1 << PT_Flag::Present)) != 0) {
		return ok;
	}

	ok = true;
	pml1[pml1Entry] = phys | flags;

	return ok;
}

#ifdef UNDEF
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

		if(value & (1 << PT_Flag::Present)) {
			/* page table already exists */
			value &= 0x7ffffffffffff000;
		} else {
			/* allocate a new page table */
			value = AllocatePage();
			*table = value | (1 << PT_Flag::Present) | (1 << PT_Flag::ReadWrite) | (1 << PT_Flag::UserSuper);
		}

		--levels;
		table = (volatile u64*)(VMM::PhysicalToVirtual(value) + ((virt >> (12 + 9 * levels)) & 0x1ff));
	}

	/* 'table' now points to the PTE we're interested in modifying */
	*table = phys | flags;
}

#endif
	
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
