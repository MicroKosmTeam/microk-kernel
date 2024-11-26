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

void Deinit() {
	PhysicalMemoryManager.IsActive = false;
}

static void CheckSpace(OBJECT_TYPE type) {
	KInfo *info = GetInfo();

	usize slots = CAPABILITY::GetFreeSlots(info->RootCSpace);
	if (slots < 10) {
		usize splitCount = 1;
		usize splitSize = PAGE_SIZE;
		Capability *splitArray[splitCount];
		PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

		usize retypeCount = 1;
		Capability *nodeRetypeArray[retypeCount];
		CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CAPABILITY_NODE, retypeCount, nodeRetypeArray);
		CAPABILITY::AddSlabNode(info->RootCSpace, nodeRetypeArray[0]);
	}
}

void *RequestPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(FRAME_MEMORY);

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
	
	CheckSpace(VIRTUAL_MEMORY_PAGING_STRUCTURE);

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
	
	CheckSpace(FRAME_MEMORY);

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
