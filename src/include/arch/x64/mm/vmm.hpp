#pragma once
#include <init/kinfo.hpp>
#include <mm/memory.hpp>
#include <init/kinfo.hpp>

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

	__attribute__((always_inline))
	uptr NewVirtualSpace();

	__attribute__((always_inline))
	void LoadVirtualSpace(uptr topLevel);

	void MapPage(uptr rootPageTable, uptr virt, uptr phys, usize flags);
}
