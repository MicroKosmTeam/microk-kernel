#include <mm/memory.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mm/bootmem.hpp>
#include <mm/slab.hpp>
#include <mm/heap.hpp>
#include <init/kinfo.hpp>

void *Malloc(usize size) {
	KInfo *info = GetInfo();

	if(BOOTMEM::BootmemIsActive()) {
		return BOOTMEM::Malloc(size);
	} else if (info->KernelHeap != NULL) {
		return MEM::HEAP::Alloc(info->KernelHeap, size);
	}

	return NULL;
}

void Free(void *ptr) {
	KInfo *info = GetInfo();

	if (info->KernelHeap != NULL) {
		return MEM::HEAP::Free(info->KernelHeap, ptr);
	}
}

void Free(void *ptr, usize size) {
	KInfo *info = GetInfo();

	if (info->KernelHeap != NULL) {
		return MEM::HEAP::Free(info->KernelHeap, ptr, size);
	}
}

namespace MEM {
void InvokeOOM() {
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "OOM invoked, system is out of memory.\r\n");

	PANIC("Out of memory");
}

void Init() {
	KInfo *info = GetInfo();

	/*
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Contiguous regions:\r\n");
	bool isAllUnusable = true;
	uptr base = info->MemoryMap[0].Base;
	usize length = info->MemoryMap[0].Length;

	for (usize region = 1; region < info->MemoryMapEntryCount; region++) {
		if (info->MemoryMap[region - 1].Base + info->MemoryMap[region - 1].Length == info->MemoryMap[region].Base) {
			length += info->MemoryMap[region].Length;
			if(isAllUnusable) isAllUnusable = info->MemoryMap[region].Type == MEMMAP_USABLE ? false : true;
		} else {
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME " %s area: [0x%x - 0x%x]\r\n",
				isAllUnusable ? "Unusable" : "Usable",
				base,
				base + length);

			base = info->MemoryMap[region].Base;
			length = info->MemoryMap[region].Length;
			isAllUnusable = info->MemoryMap[region].Type == MEMMAP_USABLE ? false : true;
		}
	}

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME " %s area: [0x%x - 0x%x]\r\n",
				isAllUnusable ? "Unusable" : "Usable",
				base,
				base + length);
	*/

	CatalogueKernelMemory();

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Provided physical RAM map:\r\n");

	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		PRINTK::PrintK(PRINTK_DEBUG " [0x%x - 0x%x] -> %s\r\n",
				current->Base,
				current->Base + current->Length,
				MemoryTypeToString(current->Type));
	}


	/* Enabling the page frame allocator */
	PMM::InitPageFrameAllocator();

	/* Initializing virtual memory */
	VMM::InitVMM();

	/* Initialize the slab allocator */
	info->KernelSlabAllocator = SLAB::InitializeAllocator();


	MEM::MEMBLOCK::SetupSlabAllocation(info->PhysicalMemoryChunks, info->KernelSlabAllocator);

	/* Initializing the heap */
	info->KernelHeap = HEAP::InitializeHeap(info->KernelSlabAllocator);

	/* With the heap initialized, disable new bootmem allocations */
	BOOTMEM::DeactivateBootmem();

	FreeBootMemory();
}

void CatalogueKernelMemory() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME 
			"Total kernel size: [0x%x - 0x%x] -> %d bytes\r\n"
			"        Essential: [0x%x - 0x%x] -> %d bytes\r\n"
			"             Text: [0x%x - 0x%x] -> %d bytes\r\n"
			"           ROData: [0x%x - 0x%x] -> %d bytes\r\n"
			"             Data: [0x%x - 0x%x] -> %d bytes\r\n"
			"          Dynamic: [0x%x - 0x%x] -> %d bytes\r\n"
			"              BSS: [0x%x - 0x%x] -> %d bytes\r\n",
			&__KernelBinaryEssentialStart, &__KernelBinaryBSSEnd, (&__KernelBinaryBSSEnd - &__KernelBinaryEssentialStart),
			&__KernelBinaryEssentialStart, &__KernelBinaryEssentialEnd, (&__KernelBinaryEssentialEnd - &__KernelBinaryEssentialStart),
			&__KernelBinaryTextStart, &__KernelBinaryTextEnd, (&__KernelBinaryTextEnd - &__KernelBinaryTextStart),
			&__KernelBinaryRODataStart, &__KernelBinaryRODataEnd, (&__KernelBinaryRODataEnd - &__KernelBinaryRODataStart),
			&__KernelBinaryDataStart, &__KernelBinaryDataEnd, (&__KernelBinaryDataEnd - &__KernelBinaryDataStart),
			&__KernelBinaryDynamicStart, &__KernelBinaryDynamicEnd, (&__KernelBinaryDynamicEnd - &__KernelBinaryDynamicStart),
			&__KernelBinaryBSSStart, &__KernelBinaryBSSEnd, (&__KernelBinaryBSSEnd - &__KernelBinaryBSSStart)
			);

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


	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, essentialStartAddr, essentialEndAddr - essentialStartAddr, MEMMAP_KERNEL_ESSENTIALS);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, textStartAddr, textEndAddr - textStartAddr, MEMMAP_KERNEL_TEXT);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, rodataStartAddr, rodataEndAddr - rodataStartAddr, MEMMAP_KERNEL_RODATA);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, dataStartAddr, dataEndAddr - dataStartAddr, MEMMAP_KERNEL_DATA);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, dynamicStartAddr, dynamicEndAddr - dynamicStartAddr, MEMMAP_KERNEL_DYNAMIC);
	MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, bssStartAddr, bssEndAddr - bssStartAddr, MEMMAP_KERNEL_BSS);
}

void FreeBootMemory() {
	KInfo *info = GetInfo();

	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {

		if (current->Type == MEMMAP_BOOTLOADER_RECLAIMABLE) {
			PRINTK::PrintK(PRINTK_DEBUG " Freeing boot data: [0x%x - 0x%x]\r\n",
				       current->Base, current->Base + current->Length);

			current->Type = MEMMAP_USABLE;

			Memset((void*)VMM::PhysicalToVirtual(current->Base), 0, current->Length);
			PMM::FreePages((void*)current->Base, current->Length / PAGE_SIZE);
		}
	}

}
}
