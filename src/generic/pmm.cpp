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

	usize slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED);
	if (slots < 3) {
		usize splitCount = 1;
		usize splitSize = PAGE_SIZE;
		Capability *splitArray[splitCount];
		PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

		usize retypeCount = 1;
		Capability *nodeRetypeArray[retypeCount];
		CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CNODE, retypeCount, nodeRetypeArray);
		CAPABILITY::AddSlabNode(info->RootCSpace, UNTYPED, nodeRetypeArray[0]);
	}

	slots = CAPABILITY::GetFreeSlots(info->RootCSpace, CNODE);
	if (slots < 3) {
		usize splitCount = 1;
		usize splitSize = PAGE_SIZE;
		Capability *splitArray[splitCount];
		PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

		usize retypeCount = 1;
		Capability *nodeRetypeArray[retypeCount];
		CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CNODE, retypeCount, nodeRetypeArray);
		CAPABILITY::AddSlabNode(info->RootCSpace, CNODE, nodeRetypeArray[0]);
	}

	slots = CAPABILITY::GetFreeSlots(info->RootCSpace, type);
	if (slots < 3) {
		usize splitCount = 1;
		usize splitSize = PAGE_SIZE;
		Capability *splitArray[splitCount];
		PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

		usize retypeCount = 1;
		Capability *nodeRetypeArray[retypeCount];
		CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], CNODE, retypeCount, nodeRetypeArray);
		CAPABILITY::AddSlabNode(info->RootCSpace, type, nodeRetypeArray[0]);
	}
}

void *RequestPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(FRAMES);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = 1;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAMES, retypeCount, frameRetypeArray);

	return (void*)frameRetypeArray[0]->Object;
}

void *RequestPages(usize length) {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	CheckSpace(FRAMES);

	usize splitCount = 1;
	usize splitSize = PAGE_SIZE * length;
	Capability *splitArray[splitCount];
	PhysicalMemoryManager.UntypedArea = CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);

	usize retypeCount = length;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAMES, retypeCount, frameRetypeArray);

	return (void*)frameRetypeArray[0]->Object;
}
}
