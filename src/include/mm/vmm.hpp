#ifndef MM_VMM_HPP_
#define MM_VMM_HPP_

#include <cdefs.h>
#include <cstdint.hpp>
#include <mm/memory.hpp>

#if defined(ARCH_x64)
#include <arch/x64/mm/vmm.hpp>
#endif

#define ROUND_DOWN_TO(x, y) \
	if (x % y) x -= x % y

#define ROUND_UP_TO(x, y) \
	if (x % y) x += y - x % y

#define ROUND_DOWN_TO_PAGE(x) \
	ROUND_DOWN_TO(x, PAGE_SIZE)

#define ROUND_UP_TO_PAGE(x) \
	ROUND_UP_TO(x, PAGE_SIZE)

/* Forward definition */
namespace MEM::MEMBLOCK {
	struct MemblockAllocator;
}

namespace VMM {
	inline
	usize ConvertUserFlags(usize flags) {
		usize ret = VMM_FLAGS_USER;

		if (flags & PAGE_PROTECTION_READ) {
			ret |= VMM_FLAGS_ACCESSIBLE;
		}
		
		if (flags & PAGE_PROTECTION_WRITE) {
			ret |= VMM_FLAGS_WRITE;
		}

		if (!(flags & PAGE_PROTECTION_EXEC)) {
			ret |= VMM_FLAGS_NOEXEC;
		}

		if (flags & PAGE_PROTECTION_NOCACHE) {
			ret |= VMM_FLAGS_NOCACHE;
		}

		return ret;
	}

	struct VirtualSpace {
		uptr VirtualHierarchyTop;
		MEM::MEMBLOCK::MemblockAllocator *VirtualMemoryLayout;
	};

	uptr PhysicalToVirtual(uptr value);
	uptr VirtualToPhysical(uptr value);
	
	VirtualSpace *NewVirtualSpace();
	void LoadVirtualSpace(VirtualSpace *space);

	void MapPage(VirtualSpace *space, uptr phys, uptr virt, usize flags);
	void ForkSpace(uptr newSpace, uptr oldSpace, usize flags);

	void MMap(VirtualSpace *space, uptr src, uptr dest, usize length, usize flags);

	void VMAlloc(VirtualSpace *space, uptr virt, usize length, usize flags);
	void VMCopyAlloc(VirtualSpace *space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen);

	void InitVMM();
	void PrepareKernelVirtualSpace(VirtualSpace *space);
	void PrepareUserVirtualSpace(VirtualSpace *space);
}

#endif /* MM_VMM_HPP_ */
