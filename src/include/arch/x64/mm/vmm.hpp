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

#define VMM_FLAGS_KERNEL_CODE (x86_64::PT_Flag::Present)

#define VMM_FLAGS_KERNEL_RODATA (x86_64::PT_Flag::Present | \
				 x86_64::PT_Flag::NX)

#define VMM_FLAGS_KERNEL_DATA (x86_64::PT_Flag::Present | \
			       x86_64::PT_Flag::NX | \
			       x86_64::PT_Flag::ReadWrite)

#define VMM_FLAGS_KERNEL_DEVICE (x86_64::PT_Flag::Present | \
			         x86_64::PT_Flag::NX | \
			         x86_64::PT_Flag::ReadWrite | \
				 x86_64::PT_Flag::CacheDisabled)


#define VMM_FLAGS_KERNEL_GENERIC (x86_64::PT_Flag::Present | \
			          x86_64::PT_Flag::ReadWrite)


#define VMM_FLAGS_USER_CODE (x86_64::PT_Flag::Present | \
			     x86_64::PT_Flag::UserSuper)

#define VMM_FLAGS_USER_RODATA (x86_64::PT_Flag::Present | \
				 x86_64::PT_Flag::NX | \
			         x86_64::PT_Flag::UserSuper)

#define VMM_FLAGS_USER_DATA (x86_64::PT_Flag::Present | \
			       x86_64::PT_Flag::NX | \
			       x86_64::PT_Flag::ReadWrite | \
			       x86_64::PT_Flag::UserSuper)

#define VMM_FLAGS_USER_GENERIC (x86_64::PT_Flag::Present | \
		                x86_64::PT_Flag::ReadWrite | \
				x86_64::PT_Flag::UserSuper)


	
	uptr NewVirtualSpace();
	void LoadVirtualSpace(uptr topLevel);

	void MapPage(uptr rootPageTable, uptr phys, uptr virt, usize flags);
	uptr FindMappedPage(uptr rootPageTable, uptr virt);
	void UnmapPage(uptr rootPageTable, uptr virt);
}
