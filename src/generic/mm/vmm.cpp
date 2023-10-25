#include <mm/vmm.hpp>
#include <arch/x64/dev/apic.hpp>
#include <sys/printk.hpp>

namespace VMM {
__attribute__((always_inline)) uptr PhysicalToVirtual(uptr value) {
	KInfo *info = GetInfo();

	value += info->HigherHalfMapping;

	return value;
}

__attribute__((always_inline)) uptr VirtualToPhysical(uptr value) {
	KInfo *info = GetInfo();

	value += info->HigherHalfMapping;

	return value;
}

__attribute__((always_inline))
uptr NewVirtualSpace() {
#if defined(ARCH_x64)
	return x86_64::NewVirtualSpace();
#endif
	return 0;
}

__attribute__((always_inline))
void LoadVirtualSpace(uptr space) {
#if defined(ARCH_x64)
	x86_64::LoadVirtualSpace(space);
#endif
}

__attribute__((always_inline))
void MapPage(uptr space, uptr virt, uptr phys, usize flags) {
#if defined(ARCH_x64)
	x86_64::MapPage(uptr space, virt, phys, flags);
#endif
}

void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace();
	PrepareVirtualSpace(space);

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Virtual memory subsystem initialized.\r\n");

}

void PrepareVirtualSpace(uptr space) {
	KInfo *info = GetInfo();

	/* We go through every entry in the memory map and map it in virtual memory */
	for (usize i = 0; i < info->MemoryMapEntryCount; i++) {
		MEM::MMapEntry entry = info->MemoryMap[i];

		/* We will skip any memory that is not usable by our kernel, to make the process faster */
		if (entry.type == MEMMAP_BAD_MEMORY ||
		    entry.type == MEMMAP_RESERVED ||
		    entry.type == MEMMAP_ACPI_NVS) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uptr base = entry.base - (entry.base % PAGE_SIZE);
		uptr top = base + entry.length + (entry.length % PAGE_SIZE);

		/* If it's kernel code, we will map its special location, otherwise we do the lower half and higher half mappings. */
		/* We use the kernel base to be sure we are not mapping module code over the kernel code. */
		if (entry.type == MEMMAP_KERNEL_AND_MODULES) {
			if(entry.base == info->KernelPhysicalBase) {
				for (uptr t = base; t < top; t += PAGE_SIZE){
					MapMemory(t, info->KernelVirtualBase + t - info->KernelPhysicalBase, VMM_FLAGS_KERNEL_GENERIC);

				}
			} else {
				for (uptr t = base; t < top; t += PAGE_SIZE) {
					MapMemory(t, t + info->HigherHalfMapping, VMM_FLAGS_USER_RODATA);
				}
			}
		} else if (entry.type == MEMMAP_ACPI_RECLAIMABLE) {
			for (uptr t = base; t < top; t += PAGE_SIZE) {
				MapPage(t, t + info->HigherHalfMapping, VMM_FLAGS_USER_RODATA);
			}
		} else {
			for (uptr t = base; t < top; t += PAGE_SIZE) {
				space->MapMemory(t, t + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DATA);
			}
		}
	}

	if(info->KernelHeapPageList != NULL) {
		uptr heapAddress = CONFIG_HEAP_BASE;
		for (usize heapPage = 0; heapPage < info->KernelHeapPageList->PageCount; heapPage++) {
			MapMemory(info->KernelHeapPageList->Pages[heapPage].Data.PhysicalAddress, heapAddress, VMM_FLAGS_KERNEL_DATA);
			heapAddress += PAGE_SIZE;
		}
	}

#if defined(ARCH_x64)
	for (uptr t = PAGE_SIZE; t < info->KernelStack; t+=PAGE_SIZE) {
		MapMemory(t, t, VMM_FLAGS_KERNEL_DATA);
	}
	
	MapMemory(x86_64::GetAPICBase(), x86_64::GetAPICBase() + info->HigherHalfMapping, VMM_FLAGS_KERNEL_DATA);
#endif


	return space;
}
}
