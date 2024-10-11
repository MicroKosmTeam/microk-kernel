#include <arch/x86/vmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <math.hpp>
inline __attribute__((always_inline))
void InvalidatePage(uptr virt) {
	asm volatile ("dsb ishst; tlbi vaae1is, %0; dsb ish; isb" : : "r"(virt) : "memory");
}

inline __attribute__((always_inline))
volatile u64* GetNextLevel(volatile u64* topLevel, usize idx, uptr allocateAddress) {
	if ((topLevel[idx] & (1 << AArch64::PT_Flag::Valid)) != 0) {
		return (volatile u64*)(VMM::PhysicalToVirtual(PTE_GET_ADDR(topLevel[idx])));
	}

	if (allocateAddress == 0) {
		return NULL;
	}

	Memclr((void*)VMM::PhysicalToVirtual(allocateAddress), PAGE_SIZE);

	topLevel[idx] = allocateAddress | VMM_FLAGS_USER | VMM_FLAGS_READ | VMM_FLAGS_WRITE | (1 << AArch64::PT_Flag::Table);
	return (volatile u64*)VMM::PhysicalToVirtual(allocateAddress);
}

namespace AArch64 {
uptr NewVirtualSpace(uptr frame) {
	Memclr((void*)VMM::PhysicalToVirtual(frame), PAGE_SIZE);
	return VMM::PhysicalToVirtual(frame);
}

void LoadVirtualSpace(uptr topLevel) {
	asm volatile ("msr ttbr0_el1, %0; dsb ish; isb" : : "r"(VMM::VirtualToPhysical(topLevel)) : "memory");
}

int MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags) {
	usize l3Entry = (virt >> 12) & 0x1FF;
	usize l2Entry = (virt >> 21) & 0x1FF;
	usize l1Entry = (virt >> 30) & 0x1FF;
	usize l0Entry = (virt >> 39) & 0x1FF;

	volatile u64 *l3;
	volatile u64 *l2;
	volatile u64 *l1;
	volatile u64 *l0;

	l3 = (volatile u64*)rootPageTable;

	l2 = GetNextLevel(l3, l3Entry, 0);
	if (l2 == NULL) {
		return 3;
	}

	l1 = GetNextLevel(l2, l2Entry, 0);
	if (l1 == NULL) {
		return 2;
	}

	l0 = GetNextLevel(l1, l1Entry, 0);
	if (l0 == NULL) {
		return 1;
	}

	if ((l0[l0Entry] & (1 << PT_Flag::Valid)) != 0) {
		return -EINVALID;
	}

	l0[l0Entry] = phys | flags | (1 << AArch64::PT_Flag::Valid);

	InvalidatePage(virt);

	return 0;
}

int MapIntermediateLevel(uptr rootPageTable, usize level, uptr frame, uptr virt, usize flags) {
	usize l3Entry = (virt >> 12) & 0x1FF;
	usize l2Entry = (virt >> 21) & 0x1FF;
	usize l1Entry = (virt >> 30) & 0x1FF;
	//usize l0Entry = (virt >> 39) & 0x1FF;

	volatile u64 *l3;
	volatile u64 *l2;
	volatile u64 *l1;
	volatile u64 *l0;

	l3 = (volatile u64*)rootPageTable;

	l2 = GetNextLevel(l3, l3Entry, 0);
	if (l2 == NULL) {	
		if (level == 3) {
			l2 = GetNextLevel(l3, l3Entry, frame);
			if (l2 == NULL) {
				return -ENOTPRESENT;
			}

			l3[l3Entry] |= flags;
			return 1;
		} else {
			return -ENOTPRESENT;
		}
	} else {
		if (level == 3) {
			l3[l3Entry] |= flags;
			return 1;
		}
	}

	l1 = GetNextLevel(l2, l2Entry, 0);
	if (l1 == NULL) {
		if (level == 2) {
			l1 = GetNextLevel(l2, l2Entry, frame);
			if (l1 == NULL) {
				return -ENOTPRESENT;
			}

			l2[l2Entry] |= flags;
			return 0;
		} else {
			return -ENOTPRESENT;
		}
	} else {
		if (level == 2) {
			l2[l2Entry] |= flags;
			return 1;
		}
	}

	l0 = GetNextLevel(l1, l1Entry, 0);
	if (l0 == NULL) {
		if (level == 1) {
			l0 = GetNextLevel(l1, l1Entry, frame);
			if (l0 == NULL) {
				return -ENOTPRESENT;
			}

			l1[l1Entry] |= flags;
			return 0;
		} else {
			return -ENOTPRESENT;
		}
	} else { 
		if (level == 1) {
			l1[l1Entry] |= flags;
			return 1;
		}
	}

	return 0;
}

int FlagPage(uptr rootPageTable, uptr virt, usize flags) {
	usize l3Entry = (virt >> 12) & 0x1FF;
	usize l2Entry = (virt >> 21) & 0x1FF;
	usize l1Entry = (virt >> 30) & 0x1FF;
	usize l0Entry = (virt >> 39) & 0x1FF;

	volatile u64 *l3;
	volatile u64 *l2;
	volatile u64 *l1;
	volatile u64 *l0;

	l3 = (volatile u64*)rootPageTable;

	l2 = GetNextLevel(l3, l3Entry, 0);
	if (l2 == NULL) {
		return -ENOTPRESENT;
	}

	l1 = GetNextLevel(l2, l2Entry, 0);
	if (l1 == NULL) {
		return -ENOTPRESENT;
	}

	l0 = GetNextLevel(l1, l1Entry, 0);
	if (l0 == NULL) {
		return -ENOTPRESENT;
	}

	if ((l0[l0Entry] & (1 << PT_Flag::Valid)) == 0) {
		return -EINVALID;
	}

	l0[l0Entry] = PTE_GET_ADDR(l0[l0Entry]) | flags;

	InvalidatePage(virt);

	return 0;
}

int UnmapPage(uptr rootPageTable, uptr virt) {
	usize l3Entry = (virt >> 12) & 0x1FF;
	usize l2Entry = (virt >> 21) & 0x1FF;
	usize l1Entry = (virt >> 30) & 0x1FF;
	usize l0Entry = (virt >> 39) & 0x1FF;

	volatile u64 *l3 = (volatile u64*)rootPageTable;

	volatile u64 *l2 = GetNextLevel(l3, l3Entry, false);
	if (l2 == NULL) {
		return -ENOTPRESENT;
	}

	volatile u64 *l1 = GetNextLevel(l2, l2Entry, false);
	if (l1 == NULL) {
		return -ENOTPRESENT;
	}

	volatile u64 *l0 = GetNextLevel(l1, l1Entry, false);
	if (l0 == NULL) {
		return -ENOTPRESENT;
	}

	if ((l0[l0Entry] & (1 << PT_Flag::Valid)) == 0) {
		return -EINVALID;
	}

	l0[l0Entry] = 0;

	InvalidatePage(virt);

	return 0;
}

volatile u64* FindMappedPTE(uptr rootPageTable, uptr virt, bool allocate) {
	usize l3Entry = (virt >> 12) & 0x1FF;
	usize l2Entry = (virt >> 21) & 0x1FF;
	usize l1Entry = (virt >> 30) & 0x1FF;
	usize l0Entry = (virt >> 39) & 0x1FF;

	volatile u64 *l3 = (volatile u64*)rootPageTable;

	volatile u64 *l2 = GetNextLevel(l3, l3Entry, allocate);
	if (l2 == NULL) {
		return NULL;
	}

	volatile u64 *l1 = GetNextLevel(l2, l2Entry, allocate);
	if (l1 == NULL) {
		return NULL;
	}

	volatile u64 *l0 = GetNextLevel(l1, l1Entry, allocate);
	if (l0 == NULL) {
		return NULL;
	}

	return &l0[l0Entry];
}

uptr FindMappedPage(uptr rootPageTable, uptr virt) {
	volatile u64 *pte = FindMappedPTE(rootPageTable, virt, false);

	if (pte == NULL || (PTE_GET_FLAGS(*pte) & (1 << PT_Flag::Valid)) == 0) {
		return -1;
	}

	return PTE_GET_ADDR(*pte);
}
}

