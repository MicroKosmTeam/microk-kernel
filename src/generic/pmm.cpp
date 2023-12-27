#include <pmm.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <kinfo.hpp>

namespace PMM {
static MEM::MEMBLOCK::MemblockRegion *FreeRegion;
static MEM::MEMBLOCK::MemblockRegion *VMAllocRegion;

static void FindNextFreeRegion() {
	KInfo *info = GetInfo();

	for (MEM::MEMBLOCK::MemblockRegion *current = (MEM::MEMBLOCK::MemblockRegion*)info->PhysicalMemoryChunks->Regions.Head;
	     current != NULL;
	     current = (MEM::MEMBLOCK::MemblockRegion*)current->Next) {
		if (current->Type != MEMMAP_USABLE) {
			continue;
		}

		FreeRegion = current;
		break;
	}

	VMAllocRegion = MEM::MEMBLOCK::AddRegion(info->PhysicalMemoryChunks, FreeRegion->Base, PAGE_SIZE, MEMMAP_KERNEL_VMALLOC);
}


void InitPageFrameAllocator(usize upperLimit) {
	(void)upperLimit;
	FreeRegion = NULL;
	VMAllocRegion = NULL;
}

void *RequestPage() {
	if (FreeRegion != NULL && FreeRegion->Length >= PAGE_SIZE) {
		uptr addr = FreeRegion->Base;

		FreeRegion->Length -= PAGE_SIZE;
		FreeRegion->Base += PAGE_SIZE;

		VMAllocRegion->Length += PAGE_SIZE;

		return (void*)addr;
	} else {
		if (VMAllocRegion != NULL) {
			VMAllocRegion->Next = FreeRegion->Next;
			FreeRegion->Next->Previous = VMAllocRegion;
		}

		FindNextFreeRegion();
		return (void*)(VMAllocRegion->Base);
	}

	PANIC("NOMEM");

	__builtin_unreachable();
}
}
