#pragma once
#include <memory.hpp>

namespace AArch64 {
	enum PT_Flag {
		Valid		  = 0,
		Table		  = 1,
		UserAccess	 = 6,
		Shareable	  = 8,
		AccessFlag	 = 10,
		NotGlobal	  = 11,
		NX			 = 54	// No execute
	};

#undef VMM_FLAGS_READ
#define VMM_FLAGS_READ       (1 << AArch64::PT_Flag::Valid)
#undef VMM_FLAGS_NOEXEC
#define VMM_FLAGS_NOEXEC     (1ULL << AArch64::PT_Flag::NX)
#undef VMM_FLAGS_WRITE
#define VMM_FLAGS_WRITE      (1 << 1)  // Write permission bit in the lower attributes
#undef VMM_FLAGS_NOCACHE
#define VMM_FLAGS_NOCACHE    (0)
#undef VMM_FLAGS_USER
#define VMM_FLAGS_USER       (1 << AArch64::PT_Flag::UserAccess)
#undef VMM_FLAGS_SHAREABLE
#define VMM_FLAGS_SHAREABLE  (1 << AArch64::PT_Flag::Shareable)
#undef VMM_FLAGS_HUGE
#define VMM_FLAGS_HUGE       (0)

#undef PTE_ADDR_MASK 
#define PTE_ADDR_MASK 0x0000FFFFFFFFF000
#undef PTE_GET_ADDR
#define PTE_GET_ADDR(VALUE) ((VALUE) & PTE_ADDR_MASK)
#undef PTE_GET_FLAGS
#define PTE_GET_FLAGS(VALUE) ((VALUE) & ~PTE_ADDR_MASK)

	uptr NewVirtualSpace(uptr frame);
	void LoadVirtualSpace(uptr topLevel);

	int MapIntermediateLevel(uptr rootPageTable, usize level, uptr frame, uptr virt, usize flags);
	int MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags);
	
	int UnmapIntermediateLevel(uptr rootPageTable, uptr virt);
	int UnmapPage(uptr rootPageTable, uptr virt);

	int FlagPage(uptr rootPageTable, uptr virt, usize flags);

	volatile u64 *FindMappedPTE(uptr rootPageTable, uptr virt, bool allocate);
	uptr FindMappedPage(uptr rootPageTable, uptr virt);
}
