#include <pmm.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <kinfo.hpp>

namespace PMM {
PhysicalMemoryManagerStruct PhysicalMemoryManager;

void Init(Capability *untypedArea) {
	PhysicalMemoryManager.UntypedArea = untypedArea;

	PhysicalMemoryManager.IsActive = true;
}

void Deinit() {
	PhysicalMemoryManager.IsActive = false;
}

void *RequestPage() {
	if (!PhysicalMemoryManager.IsActive) {
		return NULL;
	}

	KInfo *info = GetInfo();
	
	/*
	usize slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED);
	if (slots < 2) {
	
	}

	slots = CAPABILITY::GetFreeSlots(info->RootCSpace, FRAMES);
	if (slots < 2) {

	}*/

	usize splitCount = 2;
	usize splitSize = PAGE_SIZE;
	Capability *splitArray[splitCount];
	CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);
	PhysicalMemoryManager.UntypedArea = splitArray[1];

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
	
	/*
	usize slots = CAPABILITY::GetFreeSlots(info->RootCSpace, UNTYPED);
	if (slots < 2) {
	
	}

	slots = CAPABILITY::GetFreeSlots(info->RootCSpace, FRAMES);
	if (slots < 2) {

	}*/

	usize splitCount = 2;
	usize splitSize = PAGE_SIZE * length;
	Capability *splitArray[splitCount];
	CAPABILITY::SplitUntyped(info->RootCSpace, PhysicalMemoryManager.UntypedArea, splitSize, splitCount, splitArray);
	PhysicalMemoryManager.UntypedArea = splitArray[1];

	usize retypeCount = length;
	Capability *frameRetypeArray[retypeCount];
	CAPABILITY::RetypeUntyped(info->RootCSpace, splitArray[0], FRAMES, retypeCount, frameRetypeArray);

	return (void*)frameRetypeArray[0]->Object;
}
}
