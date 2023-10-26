#include <mm/vmm.hpp>
#include <arch/x64/dev/apic.hpp>
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
	//PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " -> !! Phys: 0x%x Virt 0x%x\r\n", phys, virt);
#if defined(ARCH_x64)
	x86_64::MapPage(space, phys, virt, flags);
#endif
}

void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace();
	PrepareVirtualSpace(info->KernelVirtualSpace);

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Virtual memory subsystem initialized.\r\n");

}

void PrepareVirtualSpace(uptr space) {
	KInfo *info = GetInfo();
		
	/* We go through every entry in the memory map and map it in virtual memory */
	for (usize i = 0; i < info->MemoryMapEntryCount; i++) {
		MEM::MMapEntry entry = info->MemoryMap[i];

		/* We will skip any memory that is not usable by our kernel, to make the process faster */
		if (entry.Type == MEMMAP_BAD_MEMORY ||
		    entry.Type == MEMMAP_RESERVED ||
		    entry.Type == MEMMAP_ACPI_NVS) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = entry.AddressBase - (entry.AddressBase % PAGE_SIZE);
		uptr top = base + entry.Length + (entry.Length % PAGE_SIZE);
		
		PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " !! MAPPING [0x%x - 0x%x] -> %s.\r\n", base, top, MEM::MemoryTypeToString(entry.Type));

		/* If it's kernel code, we will map its special location, otherwise we do the lower half and higher half mappings. */
		/* We use the kernel base to be sure we are not mapping module code over the kernel code. */
		if (entry.Type == MEMMAP_KERNEL_AND_MODULES) {
			if(entry.AddressBase == info->KernelPhysicalBase) {
				for (uptr t = base; t < top; t += PAGE_SIZE){
					MapPage(space, t, info->KernelVirtualBase + t - info->KernelPhysicalBase, VMM_FLAGS_KERNEL_GENERIC);
				}
			} else {
				for (uptr t = base; t < top; t += PAGE_SIZE) {
					MapPage(space, t, t + info->HigherHalfMapping, VMM_FLAGS_USER_RODATA);
				}
			}
		} else if (entry.Type == MEMMAP_ACPI_RECLAIMABLE) {
			for (uptr t = base; t < top; t += PAGE_SIZE) {
				MapPage(space, t, t + info->HigherHalfMapping, VMM_FLAGS_USER_RODATA);
			}
		} else {
			for (uptr t = base; t < top; t += PAGE_SIZE) {
				MapPage(space, t, t + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DATA);
			}
		}
	}
	
	
	if(info->KernelHeapPageList != NULL) {
		uptr heapAddress = CONFIG_HEAP_BASE;
		for (usize heapPage = 0; heapPage < info->KernelHeapPageList->PageCount; heapPage++) {
			MapPage(space, info->KernelHeapPageList->Pages[heapPage].Data.PhysicalAddress, heapAddress, VMM_FLAGS_KERNEL_DATA);
			heapAddress += PAGE_SIZE;
		}
	}
#if defined(ARCH_x64)
	for (uptr t = PAGE_SIZE; t < info->KernelStack - info->HigherHalfMapping; t+=PAGE_SIZE) {
		MapPage(space, t - info->HigherHalfMapping, t, VMM_FLAGS_KERNEL_DATA);
	}
	
	MapPage(space, x86_64::GetAPICBase(), x86_64::GetAPICBase() + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DEVICE);
#endif
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, " !! TEST.\r\n");
	
}
}
