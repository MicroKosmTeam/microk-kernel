#include <mm/vmm.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/memblock.hpp>
#include <mm/slab.hpp>

namespace VMM {
uptr PhysicalToVirtual(uptr value) {
	KInfo *info = GetInfo();

	if (value < info->HigherHalfMapping) value += info->HigherHalfMapping;

	return value;
}

uptr VirtualToPhysical(uptr value) {
	KInfo *info = GetInfo();

	if (value > info->HigherHalfMapping) value -= info->HigherHalfMapping;

	return value;
}


VirtualSpace *NewVirtualSpace() {
	KInfo *info = GetInfo();
	VirtualSpace *space = (VirtualSpace*)MEM::SLAB::Alloc(info->VirtualSpaceCache);

	usize addressableSpace;

#if defined(__x86_64__)
	space->VirtualHierarchyTop = x86_64::NewVirtualSpace();
	addressableSpace = x86_64::GetTotalAddressableMemory();
#endif
	space->VirtualMemoryLayout = MEM::MEMBLOCK::InitializeAllocator();
	MEM::MEMBLOCK::AddRegion(space->VirtualMemoryLayout, 0, addressableSpace / 2 - PAGE_SIZE, MEMMAP_USABLE);

	return space;
}


void LoadVirtualSpace(VirtualSpace *space) {
#if defined(__x86_64__)
	x86_64::LoadVirtualSpace(space->VirtualHierarchyTop);
#endif
}


void MapPage(VirtualSpace *space, uptr phys, uptr virt, usize flags) {
#if defined(__x86_64__)
	x86_64::MapPage(space->VirtualHierarchyTop, phys, virt, flags);
#endif
}

void ForkSpace(VirtualSpace *newSpace, VirtualSpace *oldSpace, usize flags) {
#if defined(__x86_64__)
	x86_64::ForkSpace(newSpace->VirtualHierarchyTop, oldSpace->VirtualHierarchyTop, flags);
#endif
}

void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace();
	PrepareKernelVirtualSpace(info->KernelVirtualSpace);

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK_DEBUG "Virtual memory subsystem initialized.\r\n");

}

void PrepareKernelVirtualSpace(VirtualSpace *space) {
	KInfo *info = GetInfo();
	
	uptr essentialStartAddr = (uptr)&__KernelBinaryEssentialStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr essentialEndAddr = (uptr)&__KernelBinaryEssentialEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr textStartAddr = (uptr)&__KernelBinaryTextStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr textEndAddr = (uptr)&__KernelBinaryTextEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr rodataStartAddr = (uptr)&__KernelBinaryRODataStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr rodataEndAddr = (uptr)&__KernelBinaryRODataEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr dataStartAddr = (uptr)&__KernelBinaryDataStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr dataEndAddr = (uptr)&__KernelBinaryDataEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr dynamicStartAddr = (uptr)&__KernelBinaryDynamicStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr dynamicEndAddr = (uptr)&__KernelBinaryDynamicEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	uptr bssStartAddr = (uptr)&__KernelBinaryBSSStart - info->KernelVirtualBase + info->KernelPhysicalBase;
	uptr bssEndAddr = (uptr)&__KernelBinaryBSSEnd - info->KernelVirtualBase + info->KernelPhysicalBase;

	ROUND_DOWN_TO_PAGE(essentialStartAddr);
	ROUND_DOWN_TO_PAGE(textStartAddr);
	ROUND_DOWN_TO_PAGE(rodataStartAddr);
	ROUND_DOWN_TO_PAGE(dataStartAddr);
	ROUND_DOWN_TO_PAGE(dynamicStartAddr);
	ROUND_DOWN_TO_PAGE(bssStartAddr);

	ROUND_UP_TO_PAGE(essentialEndAddr);
	ROUND_UP_TO_PAGE(textEndAddr);
	ROUND_UP_TO_PAGE(rodataEndAddr);
	ROUND_UP_TO_PAGE(dataEndAddr);
	ROUND_UP_TO_PAGE(dynamicEndAddr);
	ROUND_UP_TO_PAGE(bssEndAddr);

	for (uptr phys = essentialStartAddr; phys < essentialEndAddr; phys += PAGE_SIZE) {
		MapPage(space, phys, phys - info->KernelPhysicalBase + info->KernelVirtualBase, VMM_FLAGS_KERNEL_GENERIC);
	}

	for (uptr phys = textStartAddr; phys < textEndAddr; phys += PAGE_SIZE) {
		MapPage(space, phys, phys - info->KernelPhysicalBase + info->KernelVirtualBase, VMM_FLAGS_KERNEL_CODE);
	}

	for (uptr phys = rodataStartAddr; phys < rodataEndAddr; phys += PAGE_SIZE) {
		MapPage(space, phys, phys - info->KernelPhysicalBase + info->KernelVirtualBase, VMM_FLAGS_KERNEL_RODATA);
	}


	for (uptr phys = dataStartAddr; phys < dataEndAddr; phys += PAGE_SIZE) {
		MapPage(space, phys, phys - info->KernelPhysicalBase + info->KernelVirtualBase, VMM_FLAGS_KERNEL_DATA);
	}

	for (uptr phys = dynamicStartAddr; phys < dynamicEndAddr; phys += PAGE_SIZE) {
		MapPage(space, phys, phys - info->KernelPhysicalBase + info->KernelVirtualBase, VMM_FLAGS_KERNEL_DATA);
	}

	for (uptr phys = bssStartAddr; phys < bssEndAddr; phys += PAGE_SIZE) {
		MapPage(space, phys, phys - info->KernelPhysicalBase + info->KernelVirtualBase, VMM_FLAGS_KERNEL_DATA);
	}

	/* We go through every entry in the memory map and map it in virtual memory */
	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		/* We will skip any memory that is not usable by our kernel */
		if (current->Type == MEMMAP_BAD_MEMORY ||
		    current->Type == MEMMAP_RESERVED ||
		    current->Type == MEMMAP_ACPI_NVS) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = current->Base;
		ROUND_DOWN_TO_PAGE(base);

		uptr top = base + current->Length;
		ROUND_UP_TO_PAGE(top);
		
		for (uptr addr = base; addr < top; addr += PAGE_SIZE) {
			MapPage(space, addr, addr + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DATA);
		}
	}
}

void PrepareUserVirtualSpace(VirtualSpace *space) {
	KInfo *info = GetInfo();

	ForkSpace(space, info->KernelVirtualSpace, 0);
}

void MMap(VirtualSpace *space, uptr src, uptr dest, usize length, usize flags) {
	ROUND_DOWN_TO_PAGE(src);
	ROUND_DOWN_TO_PAGE(dest);
	ROUND_UP_TO_PAGE(length);

	PRINTK::PrintK(PRINTK_DEBUG "Flags: 0x%x\r\n", flags);

	for (usize diff = 0; diff < length; diff += PAGE_SIZE) {
		MapPage(space, src + diff, dest + diff, flags);
	}
}
	
MEM::MEMBLOCK::MemblockRegion *VMAlloc(VirtualSpace *space, uptr virt, usize length, usize flags) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);
	
	MEM::MEMBLOCK::MemblockRegion *region = MEM::MEMBLOCK::AddRegion(space->VirtualMemoryLayout, virt, length, MEMMAP_KERNEL_VMALLOC);

	uptr end = virt + length;
	uptr phys;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = (uptr)PMM::RequestPage();

		if (phys < PAGE_SIZE) {
			MEM::InvokeOOM();
		}
		
		Memclr((void*)VMM::PhysicalToVirtual(phys), PAGE_SIZE);
	
		MapPage(space, phys, virt, flags);
	}

	return region;
}

MEM::MEMBLOCK::MemblockRegion *VMCopyAlloc(VirtualSpace *space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);

	MEM::MEMBLOCK::MemblockRegion *region = MEM::MEMBLOCK::AddRegion(space->VirtualMemoryLayout, virt, length, MEMMAP_KERNEL_VMALLOC);

	uptr end = virt + length;
	uptr phys;

	usize copyLen;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = (uptr)PMM::RequestPage();
		if (phys < PAGE_SIZE) {
			MEM::InvokeOOM();
		}

		Memclr((void*)VMM::PhysicalToVirtual(phys), PAGE_SIZE);

		if (virtDataStart <= virt) {
			copyLen = dataLen > PAGE_SIZE ? PAGE_SIZE : dataLen;

			Memcpy((void*)VMM::PhysicalToVirtual(phys), (u8*)data, copyLen);

			data += copyLen;
			dataLen -= copyLen;
		}
	
		MapPage(space, phys, virt, flags);
	}

	return region;
}
}