#pragma once
#include <cdefs.h>
#include <cstdint.hpp>
#include <init/kinfo.hpp>
#include <mm/memory.hpp>

#if defined(ARCH_x64)
#include <arch/x64/mm/vmm.hpp>

#define PAGE_SIZE 0x1000

#define VMM_FLAGS_KERNEL_CODE (x86_64::PT_Flag::Present)

#define VMM_FLAGS_KERNEL_RODATA (x86_64::PT_Flag::Present | \
				 x86_64::PT_Flag::NX)

#define VMM_FLAGS_KERNEL_DATA (x86_64::PT_Flag::Present | \
			       x86_64::PT_Flag::NX | \
			       x86_64::PT_Flag::ReadWrite)

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

#endif


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

	__attribute__((always_inline))
	uptr PhysicalToVirtual(uptr value);

	__attribute__((always_inline))
	uptr VirtualToPhysical(uptr value);

	__attribute__((always_inline))
	uptr NewVirtualSpace();

	__attribute__((always_inline))
	void LoadVirtualSpace(uptr space);

	__attribute__((always_inline))
	void MapPage(uptr space, uptr virt, uptr phys, usize flags);

	void InitVMM();
	void PrepareVirtualSpace(uptr space);
}
