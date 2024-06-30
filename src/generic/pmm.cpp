#include <pmm.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <kinfo.hpp>

namespace PMM {
static PhysicalMemoryManagerStruct PhysicalMemoryManager;

static void FindNextFreeRegion() {
	KInfo *info = GetInfo();

	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Tail;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Previous) {
		if (current->Type != MEMMAP_USABLE) {
			continue;
		}

		PhysicalMemoryManager.FreeRegion = current;
		break;
	}

	PhysicalMemoryManager.VMAllocRegion = MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, PhysicalMemoryManager.FreeRegion->Base, PAGE_SIZE, MEMMAP_KERNEL_VMALLOC);
}


void Init() {
	PhysicalMemoryManager.FreeRegion = NULL;
	PhysicalMemoryManager.VMAllocRegion = NULL;

	PhysicalMemoryManager.IsActive = true;
}

void Deinit() {
	PhysicalMemoryManager.IsActive = false;
}

void *RequestPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	if (PhysicalMemoryManager.FreeRegion != NULL && PhysicalMemoryManager.FreeRegion->Length >= PAGE_SIZE) {
		uptr addr = PhysicalMemoryManager.FreeRegion->Base;

		PhysicalMemoryManager.FreeRegion->Length -= PAGE_SIZE;
		PhysicalMemoryManager.FreeRegion->Base += PAGE_SIZE;

		PhysicalMemoryManager.VMAllocRegion->Length += PAGE_SIZE;

		return (void*)addr;
	} else {
		if (PhysicalMemoryManager.VMAllocRegion != NULL) {
			PhysicalMemoryManager.VMAllocRegion->Next = PhysicalMemoryManager.FreeRegion->Next;
			PhysicalMemoryManager.FreeRegion->Next->Previous = PhysicalMemoryManager.VMAllocRegion;
		}

		FindNextFreeRegion();
		return (void*)(PhysicalMemoryManager.VMAllocRegion->Base);
	}

	PANIC("NOMEM");

	__builtin_unreachable();
}

void *RequestPages(usize length) {
	OOPS("RequestPages is broken");
	/* TODO: fix */

	KInfo *info = GetInfo();
	ROUND_UP_TO_PAGE(length);

	MEM::MEMBLOCK::MemblockRegion *region = MEM::MEMBLOCK::FindFreeRegion(info->PhysicalMemoryChunks, length, true);

	if (region != NULL) {
		region = MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, region->Base, length, MEMMAP_KERNEL_VMALLOC);
		return (void*)region->Base;
	}

	PANIC("NOMEM");

	__builtin_unreachable();

}
}
