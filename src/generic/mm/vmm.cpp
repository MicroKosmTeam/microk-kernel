#include <mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/mm/vmm.hpp>
#include <arch/x64/dev/apic.hpp>
#include <sys/printk.hpp>

namespace VMM {
void InitVMM() {
	KInfo *info = GetInfo();

	info->KernelVirtualSpace = NewVirtualSpace();

	LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Virtual memory subsystem initialized.\r\n");

}

VirtualSpace *NewVirtualSpace() {
	KInfo *info = GetInfo();

	VirtualSpace *space = AllocateVirtualSpace();
	
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
					space->MapMemory((void*)t, (void*)(info->KernelVirtualBase + t - info->KernelPhysicalBase), VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL);

				}
			} else {
				for (uptr t = base; t < top; t += PAGE_SIZE) {
					space->MapMemory((void*)t, (void*)(t + info->HigherHalfMapping), VMM_PRESENT | VMM_GLOBAL | VMM_USER | VMM_NOEXECUTE);
				}
			}
		} else if (entry.type == MEMMAP_ACPI_RECLAIMABLE) {
			for (uptr t = base; t < top; t += PAGE_SIZE) {
				space->MapMemory((void*)t, (void*)(t + info->HigherHalfMapping), VMM_PRESENT | VMM_GLOBAL | VMM_USER | VMM_NOEXECUTE);
			}
		} else {
			for (uptr t = base; t < top; t += PAGE_SIZE) {
				space->MapMemory((void*)t, (void*)(t + info->HigherHalfMapping), VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL | VMM_NOEXECUTE);
			}
		}
	}

	if(info->KernelHeapPageList != NULL) {
		uptr heapAddress = CONFIG_HEAP_BASE;
		for (usize heapPage = 0; heapPage < info->KernelHeapPageList->PageCount; heapPage++) {
			space->MapMemory((void*)info->KernelHeapPageList->Pages[heapPage].Data.PhysicalAddress, (void*)heapAddress, VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL | VMM_NOEXECUTE);
			heapAddress += PAGE_SIZE;
		}
	}

#if defined(ARCH_x64)
	for (uptr t = PAGE_SIZE; t < info->KernelStack; t+=PAGE_SIZE) {
		space->MapMemory((void*)t, (void*)t, VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL | VMM_NOEXECUTE);
	}
	
	space->MapMemory((void*)x86_64::GetAPICBase(), (void*)(x86_64::GetAPICBase() + info->HigherHalfMapping), VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL | VMM_NOEXECUTE);
#endif


	return space;
}

VirtualSpace *AllocateVirtualSpace() {
#if defined(ARCH_x64)
	return x86_64::NewVirtualSpace();
#elif defined(ARCH_aarch64)
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "TODO: Implement arm virtual memory.\r\n");
	return NULL;
#endif
}

void LoadVirtualSpace(VMM::VirtualSpace *space) {
#if defined(ARCH_x64)
	x86_64::LoadVirtualSpace(space);
#elif defined(ARCH_aarch64)
	(void)space;
	return;
#endif
}

void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory) {
	space->MapMemory(physicalMemory, virtualMemory, VMM_PRESENT | VMM_READWRITE | VMM_USER);
}
	
void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory, usize flags) {
	space->MapMemory(physicalMemory, virtualMemory, flags);

}
}
