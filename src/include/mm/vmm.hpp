#pragma once
#include <cdefs.h>
#include <cstdint.hpp>
#include <mm/memory.hpp>

#if defined(ARCH_x64)
#include <arch/x64/mm/vmm.hpp>
#endif

#define ROUND_DOWN_TO(x, y) \
	x -= x % y

#define ROUND_UP_TO(x, y) \
	x += y - x % y

#define ROUND_DOWN_TO_PAGE(x) \
	ROUND_DOWN_TO(x, PAGE_SIZE)

#define ROUND_UP_TO_PAGE(x) \
	ROUND_UP_TO(x, PAGE_SIZE)

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

	inline
	int CheckValidVMMFlags(usize flags, bool isUser) {
		if (isUser) {
			if (flags != VMM_FLAGS_USER_CODE ||
			    flags != VMM_FLAGS_USER_RODATA ||
			    flags != VMM_FLAGS_USER_DATA) {
				return -EINVALID;
			} else {
				return 0;
			}
		} else {
			if (flags != VMM_FLAGS_KERNEL_CODE ||
			    flags != VMM_FLAGS_KERNEL_RODATA ||
			    flags != VMM_FLAGS_KERNEL_DATA ||
			    flags != VMM_FLAGS_KERNEL_GENERIC ||
			    flags != VMM_FLAGS_USER_CODE ||
			    flags != VMM_FLAGS_USER_RODATA ||
			    flags != VMM_FLAGS_USER_DATA ||
			    flags != VMM_FLAGS_USER_GENERIC) {
				return -EINVALID;
			} else {
				return 0;
			}
		}
	}

	uptr PhysicalToVirtual(uptr value);
	uptr VirtualToPhysical(uptr value);
	
	uptr NewVirtualSpace();
	void LoadVirtualSpace(uptr space);

	void MapPage(uptr space, uptr phys, uptr virt, usize flags);
	void ForkSpace(uptr newSpace, uptr oldSpace, usize flags);

	void MMap(uptr space, uptr src, uptr dest, usize length, usize flags);

	void VMAlloc(uptr space, uptr virt, usize length, usize flags);
	void VMCopyAlloc(uptr space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen);

	void InitVMM();
	void PrepareKernelVirtualSpace(uptr space);
	void PrepareUserVirtualSpace(uptr space);
}
