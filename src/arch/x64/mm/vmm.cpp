#include <arch/x64/mm/vmm.hpp>
#include <sys/panic.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/bootmem.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <init/kinfo.hpp>

#define PAGE_SIZE 0x1000

bool initialized = false;

extern volatile uintptr_t text_start_addr, text_end_addr, rodata_start_addr, rodata_end_addr, data_start_addr, data_end_addr;

PageTable *PML4;

namespace x86_64 {
void InitVMM() {
	if(initialized) return;

	initialized = true;

	KInfo *info = GetInfo();

	PML4 = (PageTable*)PMM::RequestPage();
	memset(PML4, 0, PAGE_SIZE);

	// Copy the bootloader page table into ours, so we know it's right
	PageTable *OldPML4;
	asm volatile("mov %%cr3, %0" : "=r"(OldPML4) : :"memory");
	for (int i = 0; i < PAGE_SIZE; i++) {
		PML4[i] = OldPML4[i];
	}

	GlobalPageTableManager = new PageTableManager(PML4);
	PRINTK::PrintK("Kernel page table initialized.\r\n");

	PT_Flag flags[128];
	bool flagStatus[128];
	uint64_t flagNumber;

/*
	for (uint64_t t = text_start_addr; t < text_end_addr; t++) {
		uint64_t phys = t - info->kernelVirtualBase + info->kernelPhysicalBase;
		GlobalPageTableManager->MapMemory((void*)t , (void*)phys);
	}

	flagNumber = 1;
	flags[0] = PT_Flag::NX;
	flagStatus[0] = true;
	for (uint64_t t = rodata_start_addr; t < rodata_end_addr; t++) {
		uint64_t phys = t - info->kernelVirtualBase + info->kernelPhysicalBase;
		GlobalPageTableManager->MapMemory((void*)t , (void*)phys, flagNumber, flags, flagStatus);
	}

	flagNumber = 2;
	flags[0] = PT_Flag::NX;
	flagStatus[0] = true;
	flags[1] = PT_Flag::ReadWrite;
	flagStatus[1] = true;
	for (uint64_t t = data_start_addr; t < data_end_addr; t++) {
		uint64_t phys = t - info->kernelVirtualBase + info->kernelPhysicalBase;
		GlobalPageTableManager->MapMemory((void*)t , (void*)phys, flagNumber, flags, flagStatus);
	}
*/

	flagNumber = 2;
	flags[0] = PT_Flag::ReadWrite;
	flagStatus[0] = true;
	flags[1] = PT_Flag::UserSuper;
	flagStatus[1] = true;

	for (uint64_t i = 0; i < info->mMapEntryCount; i++) {
		MMapEntry *entry = info->mMap[i];

		uint64_t base = entry->base;
		uint64_t top = base + entry->length;

		for (uint64_t t = base; t < top; t += 4096){
			GlobalPageTableManager->MapMemory((void*)t, (void*)t, flagNumber, flags, flagStatus);
			GlobalPageTableManager->MapMemory((void*)t + info->higherHalfMapping, (void*)t, flagNumber, flags, flagStatus);
		}
	}


	PRINTK::PrintK("Done mapping.\r\n");

	asm volatile ("mov %0, %%cr3" : : "r" (PML4));
}
}
