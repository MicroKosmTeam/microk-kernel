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
	if(BOOTMEM::BootmemIsActive()) return BOOTMEM::Malloc(size);

	return NULL;
}

void Free(void *p) {
	(void)p;
}

void *operator new(usize size) {
	return Malloc(size);
}

void *operator new[](usize size) {
	return Malloc(size);
}

void operator delete(void* p) {
	Free(p);
}

void operator delete(void* p, usize size) {
	(void)size;
	Free(p);
}

void operator delete[](void* p) {
	Free(p);
}

void operator delete[](void* p, usize size) {
	(void)size;
	Free(p);
}

namespace MEM {
void InvokeOOM() {
	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "OOM invoked, system is out of memory.\r\n");

	PANIC("Out of memory");
}

void Init() {
	KInfo *info = GetInfo();

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Provided physical RAM map:\r\n");

	for (usize i = 0; i < info->MemoryMapEntryCount; i++) {
		PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME " [0x%x - 0x%x] -> %s\r\n",
				info->MemoryMap[i].AddressBase,
				info->MemoryMap[i].AddressBase + info->MemoryMap[i].Length,
				MemoryTypeToString(info->MemoryMap[i].Type));
	}

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Contiguous regions:\r\n");
	bool isAllUnusable = true;
	uptr base = info->MemoryMap[0].AddressBase;
	usize length = info->MemoryMap[0].Length;

	for (usize region = 1; region < info->MemoryMapEntryCount; region++) {
		if (info->MemoryMap[region - 1].AddressBase + info->MemoryMap[region - 1].Length == info->MemoryMap[region].AddressBase) {
			length += info->MemoryMap[region].Length;
			if(isAllUnusable) isAllUnusable = info->MemoryMap[region].Type == MEMMAP_USABLE ? false : true;
		} else {
			PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME " %s area: [0x%x - 0x%x]\r\n",
				isAllUnusable ? "Unusable" : "Usable",
				base,
				base + length);

			base = info->MemoryMap[region].AddressBase;
			length = info->MemoryMap[region].Length;
			isAllUnusable = info->MemoryMap[region].Type == MEMMAP_USABLE ? false : true;
		}
	}

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME " %s area: [0x%x - 0x%x]\r\n",
				isAllUnusable ? "Unusable" : "Usable",
				base,
				base + length);

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


	/* Enabling the page frame allocator */
	PMM::InitPageFrameAllocator();

	/* Initializing virtual memory */
	VMM::InitVMM();

	/* Initialize the slab allocator */
	SLAB::SlabAllocator *allocator = SLAB::InitializeAllocator();

	/* Initializing the heap */
	HEAP::Heap *heap = HEAP::InitializeHeap(allocator);
	(void)heap;

	/* With the heap initialized, disable new bootmem allocations */
	BOOTMEM::DeactivateBootmem();

	FreeBootMemory();
}

void FreeBootMemory() {
	KInfo *info = GetInfo();
		
	for (usize i = 0; i < info->MemoryMapEntryCount; i++) {
		MMapEntry *entry = &info->MemoryMap[i];

		if (entry->Type == MEMMAP_BOOTLOADER_RECLAIMABLE) {
			PRINTK::PrintK(PRINTK_DEBUG " Freeing boot data: [0x%x - 0x%x]\r\n",
				       entry->AddressBase,
				       entry->AddressBase + entry->Length);

			entry->Type = MEMMAP_USABLE;

			Memset((void*)VMM::PhysicalToVirtual(entry->AddressBase),
			       0,
			       entry->Length);

			PMM::FreePages((void*)entry->AddressBase, entry->Length / PAGE_SIZE);
		}
	}

}
}
