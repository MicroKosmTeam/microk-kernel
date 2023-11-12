#include <mm/vmm.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>

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


uptr NewVirtualSpace() {
#if defined(ARCH_x64)
	return x86_64::NewVirtualSpace();
#endif
	return 0;
}


void LoadVirtualSpace(uptr space) {
#if defined(ARCH_x64)
	x86_64::LoadVirtualSpace(space);
#endif
}


void MapPage(uptr space, uptr phys, uptr virt, usize flags) {
#if defined(ARCH_x64)
	x86_64::MapPage(space, phys, virt, flags);
#endif
}

void ForkSpace(uptr newSpace, uptr oldSpace, usize flags) {
#if defined(ARCH_x64)
	x86_64::ForkSpace(newSpace, oldSpace, flags);
#endif
}

void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace();
	PrepareKernelVirtualSpace(info->KernelVirtualSpace);

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Virtual memory subsystem initialized.\r\n");

}

void PrepareKernelVirtualSpace(uptr space) {
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
	for (usize i = 0; i < info->MemoryMapEntryCount; i++) {
		MEM::MMapEntry entry = info->MemoryMap[i];

		/* We will skip any memory that is not usable by our kernel, to make the process faster */
		if (entry.Type == MEMMAP_BAD_MEMORY ||
		    entry.Type == MEMMAP_RESERVED ||
		    entry.Type == MEMMAP_ACPI_NVS) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = entry.AddressBase;
		ROUND_DOWN_TO_PAGE(base);

		uptr top = base + entry.Length;
		ROUND_UP_TO_PAGE(top);
		
		for (uptr addr = base; addr < top; addr += PAGE_SIZE) {
			MapPage(space, addr, addr + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DATA);
		}
	}
}

void PrepareUserVirtualSpace(uptr space) {
	KInfo *info = GetInfo();

	ForkSpace(space, info->KernelVirtualSpace, 0);
}

void MMap(uptr space, uptr src, uptr dest, usize length, usize flags) {
	ROUND_DOWN_TO_PAGE(src);
	ROUND_DOWN_TO_PAGE(dest);
	ROUND_UP_TO_PAGE(length);

	uptr end = src + length;

	for (; src < end; src += PAGE_SIZE, dest += PAGE_SIZE) {
		MapPage(space, src, dest, flags);
	}
}
	
void VMAlloc(uptr space, uptr virt, usize length, usize flags) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);

	uptr end = virt + length;
	uptr phys;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = (uptr)PMM::RequestPage();
	
		MapPage(space, phys, virt, flags);
	}
}

void VMCopyAlloc(uptr space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);

	uptr end = virt + length;
	uptr phys;

	usize copyLen;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = (uptr)PMM::RequestPage();
		Memset((void*)VMM::PhysicalToVirtual(phys), 0, PAGE_SIZE);

		if (virtDataStart <= virt) {
			copyLen = dataLen > PAGE_SIZE ? PAGE_SIZE : dataLen;

			Memcpy((void*)VMM::PhysicalToVirtual(phys), (u8*)data, copyLen);

			data += copyLen;
			dataLen -= copyLen;
		}
	
		MapPage(space, phys, virt, flags);
	}
}
}
