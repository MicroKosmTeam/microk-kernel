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



void CheckSpace(CapabilitySpace *cspace, usize count) {
	bool free = false;
	do {
		free = false;

		usize slots = CAPABILITY::GetFreeSlots(cspace);
		if (slots < count) {
			free = true;
			usize splitCount = 1;
			usize splitSize = PAGE_SIZE;
			Capability *splitArray[splitCount];
			PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(cspace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

			usize retypeCount = 1;
			Capability *nodeRetypeArray[retypeCount];
			CAPABILITY::RetypeUntyped(cspace, splitArray[0], CAPABILITY_NODE, retypeCount, nodeRetypeArray);
			CAPABILITY::AddSlabNode(cspace, nodeRetypeArray[0]);
		}
	} while(free);
}

void Deinit() {
	KInfo *info = GetInfo();

	// Reserve space for userspace
	CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE * 100); 
	
	PhysicalMemoryManager.IsActive = false;
}

void *RequestPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE;
	Capability *splitArray[splitCount];
	// TODO: Issue is, when tree deletes, address changes, fucking this up
	// Fix by using Address?
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = 1;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAME_MEMORY, retypeCount, frameRetypeArray);

	return (void*)VMM::PhysicalToVirtual(frameRetypeArray[0]->Object);
}

void *RequestVirtualPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = 1;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], VIRTUAL_MEMORY_PAGING_STRUCTURE, retypeCount, frameRetypeArray);

	return (void*)VMM::PhysicalToVirtual(frameRetypeArray[0]->Object);
}

void *RequestPages(usize length) {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(info->RootCSpace, length + DEFAULT_CHECK_SPACE);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE * length;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = length;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAME_MEMORY, retypeCount, frameRetypeArray);

	return (void*)VMM::PhysicalToVirtual(frameRetypeArray[0]->Object);
}
}
