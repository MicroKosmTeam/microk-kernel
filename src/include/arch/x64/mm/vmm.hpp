#pragma once
#include <mm/memory.hpp>

namespace x86_64 {
	enum PT_Flag {
        	Present       = 0,
	        ReadWrite     = 1,
        	UserSuper     = 2,
	        WriteThrough  = 3,
        	CacheDisabled = 4,
	        Accessed      = 5,
        	LargerPages   = 7,
		Global        = 8,
        	Custom0       = 9,
	        Custom1       = 10,
        	Custom2       = 11,
	        NX            = 63 // Only if supported
	};

#define PAGE_SIZE 0x1000

#define VMM_FLAGS_KERNEL_CODE (1 << x86_64::PT_Flag::Present | 0)

#define VMM_FLAGS_KERNEL_RODATA (1 << x86_64::PT_Flag::Present | \
				 1 << x86_64::PT_Flag::NX)

#define VMM_FLAGS_KERNEL_DATA (1 << x86_64::PT_Flag::Present | \
			       1 << x86_64::PT_Flag::NX | \
			       1 << x86_64::PT_Flag::ReadWrite)

#define VMM_FLAGS_KERNEL_DEVICE (1 << x86_64::PT_Flag::Present | \
			         1 << x86_64::PT_Flag::NX | \
			         1 << x86_64::PT_Flag::ReadWrite | \
				 1 << x86_64::PT_Flag::WriteThrough)


#define VMM_FLAGS_KERNEL_GENERIC (1 << x86_64::PT_Flag::Present | \
			          1 << x86_64::PT_Flag::ReadWrite)


#define VMM_FLAGS_USER_CODE (1 << x86_64::PT_Flag::Present | \
			     1 << x86_64::PT_Flag::UserSuper)

#define VMM_FLAGS_USER_RODATA (1 << x86_64::PT_Flag::Present | \
			       1 << x86_64::PT_Flag::NX | \
			       1 <<x86_64::PT_Flag::UserSuper)

#define VMM_FLAGS_USER_DATA (1 << x86_64::PT_Flag::Present | \
			     1 << x86_64::PT_Flag::NX | \
			       1 << x86_64::PT_Flag::ReadWrite | \
			       1 << x86_64::PT_Flag::UserSuper)

#define VMM_FLAGS_USER_GENERIC (1 << x86_64::PT_Flag::Present | \
		                1 << x86_64::PT_Flag::ReadWrite | \
				1 << x86_64::PT_Flag::UserSuper)

#define PTE_ADDR_MASK 0x000ffffffffff000
#define PTE_GET_ADDR(VALUE) ((VALUE) & PTE_ADDR_MASK)
#define PTE_GET_FLAGS(VALUE) ((VALUE) & ~PTE_ADDR_MASK)
	
	uptr NewVirtualSpace();
	void LoadVirtualSpace(uptr topLevel);
	
	int ForkSpace(uptr newSpace, uptr oldSpace, usize flags);
	int MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags);
	int FlagPage(uptr rootPageTable, uptr virt, usize flags);
	int UnmapPage(uptr rootPageTable, uptr virt);

	volatile u64 *FindMappedPTE(uptr rootPageTable, uptr virt, bool allocate);
	uptr FindMappedPage(uptr rootPageTable, uptr virt);
}
