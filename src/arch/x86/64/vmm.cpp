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
volatile u64 *GetNextLevel(volatile u64 *topLevel, usize idx, uptr allocateAddress) {
	if ((topLevel[idx] & (1 << x86_64::PT_Flag::Present)) != 0) {
		return (volatile u64*)(VMM::PhysicalToVirtual(PTE_GET_ADDR(topLevel[idx])));
	}

	if (allocateAddress == 0) {
		return NULL;
	}
	
	Memclr((void*)VMM::PhysicalToVirtual(allocateAddress), PAGE_SIZE);

	topLevel[idx] = allocateAddress | VMM_FLAGS_USER | VMM_FLAGS_READ | VMM_FLAGS_WRITE;
	return (volatile u64*)VMM::PhysicalToVirtual(allocateAddress);
}


namespace x86_64 {
uptr NewVirtualSpace(uptr frame) {
	/* We create a new empty page directory */
	Memclr((void*)VMM::PhysicalToVirtual(frame), PAGE_SIZE);
	return VMM::PhysicalToVirtual(frame);
}


void LoadVirtualSpace(uptr topLevel) {
	/* This loads the page directory into memory */
	asm volatile ("mov %%cr3, %0" : : "r" (VMM::VirtualToPhysical(topLevel)) : "memory");
}
	
int MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags) {
	usize pml5Entry = (virt & (0x1ffull << 48)) >> 48;
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	//volatile u64 *pml5 = (volatile u64*)rootPageTable;
	volatile u64 *pml4;
	volatile u64 *pml3;
	volatile u64 *pml2;
	volatile u64 *pml1;

	/*
	pml4 = GetNextLevel(pml5, pml5Entry, 0);
	if (pml4 == NULL) {
		return -ENOTPRESENT;
	}*/
	(void)pml5Entry;
	pml4 = (volatile u64*)rootPageTable;
	
	//PRINTK::PrintK(//PRINTK_DEBUG "PML4: 0x%x (%d)\r\n", pml4, pml4Entry);
	pml3 = GetNextLevel(pml4, pml4Entry, 0);
	if (pml3 == NULL) {
		return 3;
	}

	//PRINTK::PrintK(//PRINTK_DEBUG "PML3: 0x%x (%d)\r\n", pml3, pml3Entry);
	pml2 = GetNextLevel(pml3, pml3Entry, 0);
	if (pml2 == NULL) {
		return 2;
	}

	//PRINTK::PrintK(//PRINTK_DEBUG "PML2: 0x%x (%d)\r\n", pml2, pml2Entry);
	pml1 = GetNextLevel(pml2, pml2Entry, 0);
	if (pml1 == NULL) {
		return 1;
	}


	//PRINTK::PrintK(//PRINTK_DEBUG "PML1: 0x%x (%d)\r\n", pml1, pml1Entry);
	if ((pml1[pml1Entry] & (1 << PT_Flag::Present)) != 0) {
		return -EINVALID;
	}
	
	pml1[pml1Entry] = phys | flags;
	
	InvalidatePage(virt);

	return 0;
}
/* Returns 1 if the frame wasn't used */
int MapIntermediateLevel(uptr rootPageTable, usize level, uptr frame, uptr virt, usize flags) {
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	//usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4;
	volatile u64 *pml3;
	volatile u64 *pml2;
	volatile u64 *pml1;

	pml4 = (volatile u64*)rootPageTable;

	//PRINTK::PrintK(PRINTK_DEBUG "PML4: 0x%x (%d)\r\n", pml4, pml4Entry);
	pml3 = GetNextLevel(pml4, pml4Entry, 0);
	if (pml3 == NULL) {	
		if (level == 3) {
			pml3 = GetNextLevel(pml4, pml4Entry, frame);
			if (pml3 == NULL) {
				return -ENOTPRESENT;
			}

			pml4[pml4Entry] |= flags;
			return 1;
		} else {
			return -ENOTPRESENT;
		}
	} else {
		if (level == 3) {
			pml4[pml4Entry] |= flags;
			return 1;
		}
	}

	//PRINTK::PrintK(PRINTK_DEBUG "PML3: 0x%x (%d)\r\n", pml3, pml3Entry);
	pml2 = GetNextLevel(pml3, pml3Entry, 0);
	if (pml2 == NULL) {
		if (level == 2) {
			pml2 = GetNextLevel(pml3, pml3Entry, frame);
			if (pml2 == NULL) {
				return -ENOTPRESENT;
			}

			pml3[pml3Entry] |= flags;
			return 0;
		} else {
			return -ENOTPRESENT;
		}
	} else {
		if (level == 2) {
			pml3[pml3Entry] |= flags;
			return 1;
		}
	}
	
	//PRINTK::PrintK(PRINTK_DEBUG "PML2: 0x%x (%d)\r\n", pml2, pml2Entry);
	pml1 = GetNextLevel(pml2, pml2Entry, 0);
	if (pml1 == NULL) {
		if (level == 1) {
			pml1 = GetNextLevel(pml2, pml2Entry, frame);
			if (pml1 == NULL) {
				return -ENOTPRESENT;
			}

			pml2[pml2Entry] |= flags;
			return 0;
		} else {
			return -ENOTPRESENT;
		}
	} else { 
		if (level == 1) {
			pml2[pml2Entry] |= flags;
			return 1;
		}
	}


	return 0;
}

int FlagPage(uptr rootPageTable, uptr virt, usize flags) {
	usize pml4Entry = (virt & (0x1ffull << 39)) >> 39;
	usize pml3Entry = (virt & (0x1ffull << 30)) >> 30;
	usize pml2Entry = (virt & (0x1ffull << 21)) >> 21;
	usize pml1Entry = (virt & (0x1ffull << 12)) >> 12;

	volatile u64 *pml4;
	volatile u64 *pml3;
	volatile u64 *pml2;
	volatile u64 *pml1;

	pml4 = (volatile u64*)rootPageTable;

	pml3 = GetNextLevel(pml4, pml4Entry, 0);
	if (pml3 == NULL) {
		return -ENOTPRESENT;
	}

	pml2 = GetNextLevel(pml3, pml3Entry, 0);
	if (pml2 == NULL || pml3[pml3Entry] & (1 << PT_Flag::HugePages)) {
		return -ENOTPRESENT;
	}

	pml1 = GetNextLevel(pml2, pml2Entry, 0);
	if (pml1 == NULL || pml2[pml2Entry] & (1 << PT_Flag::HugePages)) {
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

uptr FindMappedPage(uptr rootPageTable, uptr virt) {
	volatile u64 *pte = FindMappedPTE(rootPageTable, virt, false);

	if (pte == NULL || (PTE_GET_FLAGS(*pte) & (1 << PT_Flag::Present)) == 0) {
		return -1;
	}

	return PTE_GET_ADDR(*pte);
}
}
