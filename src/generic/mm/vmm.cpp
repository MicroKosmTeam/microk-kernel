#include <mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/mm/vmm.hpp>
#include <sys/printk.hpp>

namespace VMM {
void InitVMM() {
	KInfo *info = GetInfo();

	info->kernelVirtualSpace = NewKernelVirtualSpace();

	LoadVirtualSpace(info->kernelVirtualSpace);

	PRINTK::PrintK("Virtual memory subsystem initialized.\r\n");

}

VirtualSpace *NewModuleVirtualSpace() {
	KInfo *info = GetInfo();

	VirtualSpace *space = NewVirtualSpace();

	/* We go through every entry in the memory map and map it in virtual memory */
	for (int i = 0; i < info->mMapEntryCount; i++) {
		MEM::MMapEntry entry = info->mMap[i];

		if (entry.type != MEMMAP_KERNEL_AND_MODULES && entry.type != MEMMAP_BOOTLOADER_RECLAIMABLE) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uint64_t base = entry.base - (entry.base % PAGE_SIZE);
		uint64_t top = base + entry.length + (entry.length % PAGE_SIZE);

		/* If it's kernel code, we will map its special location, otherwise we do the lower half and higher half mappings. */
		/* We use the kernel base to be sure we are not mapping module code over the kernel code. */
		if (entry.type == MEMMAP_KERNEL_AND_MODULES && entry.base == info->kernelPhysicalBase) {
			for (uint64_t t = base; t < top; t += PAGE_SIZE){
				space->MapMemory(t, info->kernelVirtualBase + t - info->kernelPhysicalBase, VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL);

			}
		} else {
			for (uint64_t t = base; t < top; t += PAGE_SIZE) {
				space->MapMemory(t, t + info->higherHalfMapping, VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL);
				space->MapMemory(t, t, VMM_PRESENT | VMM_READWRITE | VMM_USER | VMM_GLOBAL);
			}
		}
	}

#ifdef CONFIG_ARCH_x86_64
	for (uintptr_t t = PAGE_SIZE; t < info->kernelStack; t+=PAGE_SIZE) {
		space->MapMemory(t, t, VMM_PRESENT | VMM_READWRITE);
	}
#endif

	return space;
}

VirtualSpace *NewKernelVirtualSpace() {
	KInfo *info = GetInfo();

	VirtualSpace *space = NewVirtualSpace();
	
	/* We go through every entry in the memory map and map it in virtual memory */
	for (int i = 0; i < info->mMapEntryCount; i++) {
		MEM::MMapEntry entry = info->mMap[i];

		/* We will skip any memory that is not usable by our kernel, to make the process faster */
		/* We also ignore ACPI, as our kernel is not interested by the information contained in those structures */
		/* It is the responsibility of the modules to manage any ACPI related code, and also to free reclaimable areas */
		if (entry.type == MEMMAP_BAD_MEMORY ||
		    entry.type == MEMMAP_RESERVED) continue;

		/* We find the base and the top by rounding to the closest page boundary */
		uint64_t base = entry.base - (entry.base % PAGE_SIZE);
		uint64_t top = base + entry.length + (entry.length % PAGE_SIZE);

		/* If it's kernel code, we will map its special location, otherwise we do the lower half and higher half mappings. */
		/* We use the kernel base to be sure we are not mapping module code over the kernel code. */
		if (entry.type == MEMMAP_KERNEL_AND_MODULES && entry.base == info->kernelPhysicalBase) {
			for (uint64_t t = base; t < top; t += PAGE_SIZE){
				space->MapMemory(t, info->kernelVirtualBase + t - info->kernelPhysicalBase, VMM_PRESENT | VMM_READWRITE | VMM_GLOBAL);

			}
		} else {
			for (uint64_t t = base; t < top; t += PAGE_SIZE){
				space->MapMemory(t, t, VMM_PRESENT | VMM_READWRITE);
				space->MapMemory(t, t + info->higherHalfMapping, VMM_PRESENT | VMM_READWRITE);
			}
		}
	}

	return space;
}

VirtualSpace *NewVirtualSpace() {
	return x86_64::NewVirtualSpace();
}

void LoadVirtualSpace(VMM::VirtualSpace *space) {
	x86_64::LoadVirtualSpace(space);
}

void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory) {
	space->MapMemory(physicalMemory, virtualMemory, VMM_PRESENT | VMM_READWRITE | VMM_USER);
}
	
void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory, uint64_t flags) {
	space->MapMemory(physicalMemory, virtualMemory, flags);

}
}
