#pragma once
#include <init/kinfo.hpp>

namespace x86_64 {
	enum PT_Flag {
        	Present = 1 << 0,
	        ReadWrite = 1 << 1,
        	UserSuper = 1 << 2,
	        WriteThrough = 1 << 3,
        	CacheDisabled = 1 << 4,
	        Accessed = 1 << 5,
        	LargerPages = 1 << 7,
		Global = 1 << 8,
        	Custom0 = 1 << 9,
	        Custom1 = 1 << 10,
        	Custom2 = 1 << 11,
	        NX = 1 << 63 // Only if supported
	};

	inline uptr NewVirtualSpace() {
		/* We create a new empty page directory */
		uptr table = AllocatePage();
		table = VMM::PhysicalToVirtual(table);

		return table;
	}

	inline void LoadVirtualSpace(uptr topLevel) {
		/* This loads the page directory into memory */
		asm volatile ("mov %0, %%cr3" : : "r" (VMM::VirtualToPhysical(topLevel)) : "memory");
	}

	void MapPage(uptr rootPageTable, uptr virt, uptr phys, u64 flags);
}
