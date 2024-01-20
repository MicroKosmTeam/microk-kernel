#include <vmm.hpp>
#include <pmm.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <kinfo.hpp>
#include <memblock.hpp>
#include <bootmem.hpp>

namespace VMM {
void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace((uptr)PMM::RequestPage());
	PrepareKernelVirtualSpace(info->KernelVirtualSpace);

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK_DEBUG "Virtual memory subsystem initialized.\r\n");

}

void PrepareKernelVirtualSpace(VirtualSpace space) {
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

	MMap(space, essentialStartAddr, essentialStartAddr - info->KernelPhysicalBase + info->KernelVirtualBase, essentialEndAddr - essentialStartAddr, VMM_FLAGS_READ | VMM_FLAGS_WRITE);
	MMap(space, textStartAddr, textStartAddr - info->KernelPhysicalBase + info->KernelVirtualBase, textEndAddr - textStartAddr, VMM_FLAGS_READ);
	MMap(space, rodataStartAddr, rodataStartAddr - info->KernelPhysicalBase + info->KernelVirtualBase, rodataEndAddr - rodataStartAddr, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);
	MMap(space, dataStartAddr, dataStartAddr - info->KernelPhysicalBase + info->KernelVirtualBase, dataEndAddr - dataStartAddr, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
	MMap(space, bssStartAddr, bssStartAddr - info->KernelPhysicalBase + info->KernelVirtualBase, bssEndAddr - bssStartAddr, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
	
	/* We go through every entry in the memory map and map it in virtual memory */
	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {

		/* We will skip any memory that is not usable by our kernel */
		if (current->Type == MEMMAP_BAD_MEMORY ||
		    current->Type == MEMMAP_RESERVED ||
		    current->Type == MEMMAP_ACPI_NVS ||
		    current->Type == MEMMAP_ACPI_RECLAIMABLE) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = current->Base;
		ROUND_DOWN_TO_PAGE(base);

		uptr top = base + current->Length;
		ROUND_UP_TO_PAGE(top);
	
		MMap(space, base, base + info->HigherHalfMapping, top - base, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
	}
}

void PrepareUserVirtualSpace(VirtualSpace space) {
	PrepareKernelVirtualSpace(space);
}

void MMap(VirtualSpace space, uptr src, uptr dest, usize length, usize flags) {
	ROUND_DOWN_TO_PAGE(src);
	ROUND_DOWN_TO_PAGE(dest);
	ROUND_UP_TO_PAGE(length);

	for (usize diff = 0; diff < length;) {
		int result;

		do {
			result = MapPage(space, src + diff, dest + diff, flags);

			if (result == -ENOTPRESENT) {
				PANIC("PAGING");
			}

			if (result != 0) {
				//PRINTK::PrintK(PRINTK_DEBUG "Missing level: %d.\r\n", result);
				MapIntermediateLevel(space, result, (uptr)PMM::RequestPage(), dest + diff, flags);
			} else {
				break;
			}
		} while (true);

		diff += PAGE_SIZE;
	}
}
	
void VMAlloc(VirtualSpace space, uptr virt, usize length, usize flags) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);
	
	uptr end = virt + length;
	uptr phys;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = (uptr)PMM::RequestPage();

		Memclr((void*)VMM::PhysicalToVirtual(phys), PAGE_SIZE);
	
		MapPage(space, phys, virt, flags);
	}
}

void VMCopyAlloc(VirtualSpace space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);

	uptr end = virt + length;
	uptr phys;

	usize copyLen;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = (uptr)PMM::RequestPage();

		Memclr((void*)VMM::PhysicalToVirtual(phys), PAGE_SIZE);

		if (virtDataStart <= virt) {
			copyLen = dataLen > PAGE_SIZE ? PAGE_SIZE : dataLen;

			Memcpy((void*)VMM::PhysicalToVirtual(phys), (u8*)data, copyLen);

			data += copyLen;
			dataLen -= copyLen;
		}
	
		MapPage(space, phys, virt, flags);
	}
}

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


VirtualSpace NewVirtualSpace(uptr frame) {
#if defined(__x86_64__)
	return x86_64::NewVirtualSpace(frame);
#endif
	return 0;
}


void LoadVirtualSpace(VirtualSpace space) {
#if defined(__x86_64__)
	x86_64::LoadVirtualSpace(space);
#endif
}
	
int MapIntermediateLevel(uptr rootPageTable, usize level, uptr frame, uptr virt, usize flags) {
#if defined(__x86_64__)
	return x86_64::MapIntermediateLevel(rootPageTable, level, frame, virt, flags);
#endif
	return 0;
}


int MapPage(VirtualSpace space, uptr phys, uptr virt, usize flags) {
#if defined(__x86_64__)
	return x86_64::MapPage(space, phys, virt, flags);
#endif
	return 0;
}
}
