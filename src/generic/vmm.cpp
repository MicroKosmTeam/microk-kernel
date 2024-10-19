#include <vmm.hpp>
#include <printk.hpp>
#include <pmm.hpp>
#include <panic.hpp>
#include <kinfo.hpp>

namespace VMM {
void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace((uptr)PMM::RequestPage());
	PrepareKernelVirtualSpace(info->KernelVirtualSpace);

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK_DEBUG "Virtual memory subsystem initialized.\r\n");

}

void PrepareKernelVirtualSpace(VirtualSpace space) {
	PRINTK::PrintK(PRINTK_DEBUG "Preparing kernel virtual space at 0x%x\r\n", space);
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
	for (usize i = 0; info->MemoryMap[i].Address != (uptr)-1; i++) {	
		UntypedHeader *current = &info->MemoryMap[i];

		/* We will skip any memory that is not usable by our kernel */
		if (current->Flags == MEMMAP_BAD_MEMORY ||
		    current->Flags == MEMMAP_RESERVED ||
		    current->Flags == MEMMAP_ACPI_NVS ||
		    current->Flags == MEMMAP_ACPI_RECLAIMABLE) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = current->Address;
		ROUND_DOWN_TO_PAGE(base);

		uptr top = base + current->Length;
		ROUND_UP_TO_PAGE(top);
	
		MMap(space, base, base + info->HigherHalfMapping, top - base, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
	}
}

void PrepareUserVirtualSpace(VirtualSpace space) {
	PRINTK::PrintK(PRINTK_DEBUG "Preparing user virtual space at 0x%x\r\n", space);
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
	for (usize i = 0; info->MemoryMap[i].Address != (uptr)-1; i++) {	
		UntypedHeader *current = &info->MemoryMap[i];
		/* We will skip any memory that is not usable by our kernel */
		if (current->Flags == MEMMAP_BAD_MEMORY ||
		    current->Flags == MEMMAP_RESERVED ||
		    current->Flags == MEMMAP_ACPI_NVS ||
		    current->Flags == MEMMAP_ACPI_RECLAIMABLE) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = current->Address;
		ROUND_DOWN_TO_PAGE(base);

		uptr top = base + current->Length;
		ROUND_UP_TO_PAGE(top);
	
		MMap(space, base, base + info->HigherHalfMapping, top - base, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
	}

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
				MapIntermediateLevel(space, result, VMM::VirtualToPhysical((uptr)PMM::RequestPage()), dest + diff, flags);
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

	PRINTK::PrintK(PRINTK_DEBUG "VMalloc in space 0x%x, [0x%x - 0x%x], 0x%x\r\n", space, virt, end, flags);

	for (; virt < end; virt += PAGE_SIZE) {
		phys = VMM::VirtualToPhysical((uptr)PMM::RequestPage());

		Memclr((void*)VMM::PhysicalToVirtual(phys), PAGE_SIZE);
	
		MMap(space, phys, virt, PAGE_SIZE, flags);
	}
}

void VMCopyAlloc(VirtualSpace space, uptr virt, usize length, usize flags, uptr data, uptr virtDataStart, usize dataLen) {
	ROUND_DOWN_TO_PAGE(virt);
	ROUND_UP_TO_PAGE(length);

	uptr end = virt + length;
	uptr phys;

	usize copyLen;

	for (; virt < end; virt += PAGE_SIZE) {
		phys = VMM::VirtualToPhysical((uptr)PMM::RequestPage());

		Memclr((void*)VMM::PhysicalToVirtual(phys), PAGE_SIZE);

		if (virtDataStart <= virt) {
			copyLen = dataLen > PAGE_SIZE ? PAGE_SIZE : dataLen;

			Memcpy((void*)VMM::PhysicalToVirtual(phys), (u8*)data, copyLen);

			data += copyLen;
			dataLen -= copyLen;
		}
	
		MMap(space, phys, virt, PAGE_SIZE, flags);
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
#else
	/*TODO*/
	(void)frame;
#endif
	return 0;
}


void LoadVirtualSpace(VirtualSpace space) {
#if defined(__x86_64__)
	x86_64::LoadVirtualSpace(space);
#else
	/*TODO*/
	(void)space;
#endif
}
	
int MapIntermediateLevel(VirtualSpace space, usize level, uptr frame, uptr virt, usize flags) {
#if defined(__x86_64__)
	return x86_64::MapIntermediateLevel(space, level, frame, virt, flags);
#else
	/*TODO*/
	(void)space;
	(void)level;
	(void)frame;
	(void)virt;
	(void)flags;
#endif
	return 0;
}


int MapPage(VirtualSpace space, uptr phys, uptr virt, usize flags) {
#if defined(__x86_64__)
	return x86_64::MapPage(space, phys, virt, flags);
#else
	/*TODO*/
	(void)space;
	(void)phys;
	(void)virt;
	(void)flags;
#endif
	return 0;
}

uptr FindMappedPage(VirtualSpace space, uptr virt) {
#if defined(__x86_64__)
	return x86_64::FindMappedPage(space, virt);
#else
	/*TODO*/
	(void)space;
	(void)virt;
#endif
	return 0;

}
}
