#include <pmm.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <kinfo.hpp>

namespace PMM {
PhysicalMemoryManagerStruct PhysicalMemoryManager;

void Init(Capability *untypedArea) {
	PRINTK::PrintK(PRINTK_DEBUG "Untyped area: 0x%x\r\n", untypedArea);
	PhysicalMemoryManager.UntypedArea = untypedArea;

	PhysicalMemoryManager.IsActive = true;
}



static void CheckSpace(OBJECT_TYPE type, usize count) {
	KInfo *info = GetInfo();

	bool free = false;
	do {
		free = false;

		usize slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED_FRAMES);
		if (slots < count) {
			free = true;
			usize splitCount = 1;
			usize splitSize = PAGE_SIZE;
			Capability *splitArray[splitCount];
			PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

			usize retypeCount = 1;
			Capability *nodeRetypeArray[retypeCount];
			CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CAPABILITY_NODE, retypeCount, nodeRetypeArray);
			CAPABILITY::AddSlabNode(info->RootCSpace, UNTYPED_FRAMES, nodeRetypeArray[0]);
		}

		slots = CAPABILITY::GetFreeSlots(info->RootCSpace, CAPABILITY_NODE);
		if (slots < count) {
			free = true;
			usize splitCount = 1;
			usize splitSize = PAGE_SIZE;
			Capability *splitArray[splitCount];
			PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

			usize retypeCount = 1;
			Capability *nodeRetypeArray[retypeCount];
			CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CAPABILITY_NODE, retypeCount, nodeRetypeArray);
			CAPABILITY::AddSlabNode(info->RootCSpace, CAPABILITY_NODE, nodeRetypeArray[0]);
		}

		slots = CAPABILITY::GetFreeSlots(info->RootCSpace, type);
		if (slots < count) {
			free = true;
			usize splitCount = 1;
			usize splitSize = PAGE_SIZE;
			Capability *splitArray[splitCount];
			PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

			usize retypeCount = 1;
			Capability *nodeRetypeArray[retypeCount];
			CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CAPABILITY_NODE, retypeCount, nodeRetypeArray);
			CAPABILITY::AddSlabNode(info->RootCSpace, type, nodeRetypeArray[0]);
		}
	} while(free);
}

void Deinit() {
	PhysicalMemoryManager.IsActive = false;
	
	CheckSpace(FRAME_MEMORY, 300);
	CheckSpace(VIRTUAL_MEMORY_PAGING_STRUCTURE, 300);
}

void *RequestPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(FRAME_MEMORY, 3);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = 1;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAME_MEMORY, retypeCount, frameRetypeArray);

	return (void*)frameRetypeArray[0]->Object;
}

void *RequestVirtualPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(VIRTUAL_MEMORY_PAGING_STRUCTURE, 3);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = 1;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], VIRTUAL_MEMORY_PAGING_STRUCTURE, retypeCount, frameRetypeArray);

	return (void*)frameRetypeArray[0]->Object;
}

void *RequestPages(usize length) {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(FRAME_MEMORY, length + 3);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE * length;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = length;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAME_MEMORY, retypeCount, frameRetypeArray);

	return (void*)frameRetypeArray[0]->Object;
}
}
