#include <arch/x86/vmm.hpp>
#include <kinfo.hpp>
#include <pmm.hpp>
#include <printk.hpp>
#include <math.hpp>

inline __attribute__((always_inline))
void InvalidatePage(uptr virt) {
	asm volatile ("invlpg [%0]" : : "r"(virt));
}

inline __attribute__((always_inline))
uptr AllocatePage() {
	uptr address = (uptr)PMM::RequestPage();

	Memclr((void*)VMM::PhysicalToVirtual(address), PAGE_SIZE);
	return address;
}

inline __attribute__((always_inline))
volatile u64 *GetNextLevel(volatile u64 *topLevel, usize idx, bool allocate) {
	if ((topLevel[idx] & (1 << x86_64::PT_Flag::Present)) != 0) {
		return (volatile u64*)(VMM::PhysicalToVirtual(PTE_GET_ADDR(topLevel[idx])));
	}

	if (!allocate) {
		return NULL;
	}

	uptr newPage = AllocatePage();

	topLevel[idx] = newPage | VMM_FLAGS_USER | VMM_FLAGS_READ | VMM_FLAGS_WRITE;
	return (volatile u64*)VMM::PhysicalToVirtual(newPage);
}


namespace x86_64 {
uptr NewVirtualSpace() {
	/* We create a new empty page directory */
	uptr table = AllocatePage();
	table = VMM::PhysicalToVirtual(table);

	return table;
}


void LoadVirtualSpace(uptr topLevel) {
	/* This loads the page directory into memory */
	asm volatile ("mov %%cr3, %0" : : "r" (VMM::VirtualToPhysical(topLevel)) : "memory");
}
	
u64 GetTotalAddressableMemory() {
	const usize ADDRESSABLE_BITS = 48;
	return MATH::POW(2, ADDRESSABLE_BITS);
}

int ForkSpace(uptr newSpace, uptr oldSpace, usize flags) {
	(void) flags;

	volatile u64 *oldTopLevel, *newTopLevel;

	oldTopLevel = (volatile u64*)oldSpace;
	newTopLevel = (volatile u64*)newSpace;

	for (int i = 256; i < 512; ++i) {
		newTopLevel[i] = oldTopLevel[i];
	}

	return 0;
}

int MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags, bool hugerPage) {
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4 = (volatile u64*)rootPageTable;
	volatile u64 *pml3 = GetNextLevel(pml4, pml4Entry, true);
	if (pml3 == NULL) {
		return -ENOTPRESENT;
	}

	if (!hugerPage) {
		volatile u64 *pml2 = GetNextLevel(pml3, pml3Entry, true);
		if (pml2 == NULL) {
			return -ENOTPRESENT;
		}

		if ((flags & VMM_FLAGS_HUGE) == 0) {
			volatile u64 *pml1 = GetNextLevel(pml2, pml2Entry, true);
			if (pml1 == NULL) {
				return -ENOTPRESENT;
			}

			if ((pml1[pml1Entry] & (1 << PT_Flag::Present)) != 0) {
				return -EINVALID;
			}
	
			pml1[pml1Entry] = phys | flags;
		} else {
			if (virt % HUGER_PAGE_SIZE == 0) {
				pml2[pml2Entry] = phys | flags;
			}
		}
	} else {
		if (virt % HUGER_PAGE_SIZE == 0) {
			pml3[pml3Entry] = phys | flags;
		}
	}


	InvalidatePage(virt);

	return 0;
}

int FlagPage(uptr rootPageTable, uptr virt, usize flags) {
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4 = (volatile u64*)rootPageTable;
	volatile u64 *pml3 = GetNextLevel(pml4, pml4Entry, false);
	if (pml3 == NULL) {
		return -ENOTPRESENT;
	}

	volatile u64 *pml2 = GetNextLevel(pml3, pml3Entry, false);
	if (pml2 == NULL) {
		return -ENOTPRESENT;
	}

	volatile u64 *pml1 = GetNextLevel(pml2, pml2Entry, false);
	if (pml1 == NULL) {
		return -ENOTPRESENT;
	}

	if ((pml1[pml1Entry] & (1 << PT_Flag::Present)) == 0) {
		return -EINVALID;
	}

	pml1[pml1Entry] = PTE_GET_ADDR(pml1[pml1Entry]) | flags;
	
	InvalidatePage(virt);

	return 0;
}

int UnmapPage(uptr rootPageTable, uptr virt) {
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4 = (volatile u64*)rootPageTable;

	volatile u64 *pml3 = GetNextLevel(pml4, pml4Entry, false);
	if (pml3 == NULL) {
		return -ENOTPRESENT;
	}

	volatile u64 *pml2 = GetNextLevel(pml3, pml3Entry, false);
	if (pml2 == NULL) {
		return -ENOTPRESENT;
	}

	volatile u64 *pml1 = GetNextLevel(pml2, pml2Entry, false);
	if (pml1 == NULL) {
		return -ENOTPRESENT;
	}

	if ((pml1[pml1Entry] & (1 << PT_Flag::Present)) == 0) {
		return -EINVALID;
	}

	pml1[pml1Entry] = 0;

	InvalidatePage(virt);

	return 0;
}

volatile u64 *FindMappedPTE(uptr rootPageTable, uptr virt, bool allocate) {
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4 = (volatile u64*)rootPageTable;

	volatile u64 *pml3 = GetNextLevel(pml4, pml4Entry, allocate);
	if (pml3 == NULL) {
		return NULL;
	}

	volatile u64 *pml2 = GetNextLevel(pml3, pml3Entry, allocate);
	if (pml2 == NULL) {
		return NULL;
	}

	volatile u64 *pml1 = GetNextLevel(pml2, pml2Entry, allocate);
	if (pml1 == NULL) {
		return NULL;
	}

	return &pml1[pml1Entry];
}

uptr FindMappedPage(uptr rootPageTable, uintptr_t virt) {
	volatile u64 *pte = FindMappedPTE(rootPageTable, virt, false);

	if (pte == NULL || (PTE_GET_FLAGS(*pte) & (1 << PT_Flag::Present)) == 0) {
		return -1;
	}

	return PTE_GET_ADDR(*pte);
}
}
