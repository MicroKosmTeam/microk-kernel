#pragma once
#include <cdefs.h>
#include <cstdint.hpp>
#include <mm/memory.hpp>

#if defined(ARCH_x64)
#include <arch/x64/mm/vmm.hpp>
#endif

#define ROUND_DOWN_TO_PAGE(x) \
	x -= x % PAGE_SIZE

#define ROUND_UP_TO_PAGE(x) \
	x += PAGE_SIZE - x % PAGE_SIZE

namespace VMM {
	struct COWMetadata {
		uptr PhysicalAddressOfOriginal;
		uptr PhysicalAddressOfCopy;

		usize VirtualReferences;
		uptr VirtualAddresses[];
	};

	struct PageMetadata {
		bool IsCOW;
		union {
			uptr PhysicalAddress;
			COWMetadata *COW;
		} Data;
	};

	struct PageList {
		usize PageCount;
		usize AllocatedSize;
		PageMetadata Pages[];
	};

	uptr PhysicalToVirtual(uptr value);
	uptr VirtualToPhysical(uptr value);
	
	uptr NewVirtualSpace();
	void LoadVirtualSpace(uptr space);

	void MapPage(uptr space, uptr phys, uptr virt, usize flags);

	void InitVMM();
	void PrepareVirtualSpace(uptr space);
}
