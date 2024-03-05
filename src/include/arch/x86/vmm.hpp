#pragma once
#include <memory.hpp>

namespace x86_64 {
	enum PT_Flag {
        	Present       = 0,
	        ReadWrite     = 1,
        	UserSuper     = 2,
	        WriteThrough  = 3,
        	CacheDisabled = 4,
	        Accessed      = 5,
        	HugePages     = 7,
		Global        = 8,
        	Custom0       = 9,
	        Custom1       = 10,
        	Custom2       = 11,
	        NX            = 63 // Only if supported
	};

#define VMM_FLAGS_READ       ((usize)1 << x86_64::PT_Flag::Present)
#define VMM_FLAGS_NOEXEC     ((usize)1 << x86_64::PT_Flag::NX)
#define VMM_FLAGS_WRITE      ((usize)1 << x86_64::PT_Flag::ReadWrite)
#define VMM_FLAGS_NOCACHE    ((usize)1 << x86_64::PT_Flag::CacheDisabled)
#define VMM_FLAGS_USER       ((usize)1 << x86_64::PT_Flag::UserSuper)
#define VMM_FLAGS_HUGE       ((usize)1 << x86_64::PT_Flag::HugePages)

#define PTE_ADDR_MASK 0x000ffffffffff000
#define PTE_GET_ADDR(VALUE) ((VALUE) & PTE_ADDR_MASK)
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
