#pragma once
#include <cdefs.h>
#include <object.hpp>
#include <cstdint.hpp>
#include <memory.hpp>

#if defined(__x86_64__)
#include <arch/x86/vmm.hpp>
#endif

#define ROUND_DOWN_TO(x, y) \
	if (x % y) x -= x % y

#define ROUND_UP_TO(x, y) \
	if (x % y) x += y - x % y

#define ROUND_DOWN_TO_PAGE(x) \
	ROUND_DOWN_TO(x, PAGE_SIZE)

#define ROUND_UP_TO_PAGE(x) \
	ROUND_UP_TO(x, PAGE_SIZE)

namespace VMM {
	inline
	usize ConvertUserFlags(usize flags) {
		usize ret = VMM_FLAGS_USER;

		if (flags & PAGE_PROTECTION_READ) {
			ret |= VMM_FLAGS_READ;
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

	uptr PhysicalToVirtual(uptr value);
	uptr VirtualToPhysical(uptr value);
	
	VirtualSpace NewVirtualSpace(uptr frame);
	void LoadVirtualSpace(VirtualSpace space);
	
	int MapIntermediateLevel(uptr rootPageTable, usize level, uptr frame, uptr virt, usize flags);
	int MapPage(VirtualSpace space, uptr phys, uptr virt, usize flags);

	void MMap(VirtualSpace space, uptr src, uptr dest, usize length, usize flags);

	void VMAlloc(VirtualSpace space, uptr virt, usize length, usize flags);
	void VMCopyAlloc(VirtualSpace space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen);

	void InitVMM();
	void PrepareKernelVirtualSpace(VirtualSpace space);
	void PrepareUserVirtualSpace(VirtualSpace space);
}
