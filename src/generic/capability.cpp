#include <capability.hpp>
#include <memory.hpp>
#include <kinfo.hpp>
#include <container.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <slab.hpp>
#include <math.hpp>

namespace CAPABILITY {
uptr InitializeRootSpace(uptr framesBase, MemoryHeader *memoryMap) {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr containerFrame = VMM::PhysicalToVirtual(framesBase);
	Memclr((void*)containerFrame, PAGE_SIZE);
	uptr slabNodeFrame = containerFrame + PAGE_SIZE;

	/* Initializing the TCB and creating the root CSpace */
	info->RootContainer = CONTAINERS::InitializeContainer(containerFrame);
	info->RootCSpace = &info->RootContainer->CSpace;
	info->CurrentContainer = info->RootContainer;

	CapabilitySpace *space = info->RootCSpace;
	CapabilityNode *node = (CapabilityNode*)slabNodeFrame;

	space->Slabs.FreeSlabs.Head =
		space->Slabs.FreeSlabs.Tail = node;

	space->Slabs.UsedSlabs.Head =
		space->Slabs.UsedSlabs.Tail = NULL;

	space->Slabs.FullSlabs.Head =
		space->Slabs.FullSlabs.Tail = NULL;

	node->Next = node->Previous = NULL;

	node->FreeElements = CAPABILITIES_PER_NODE;

	for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
		node->Slots[i].IsMasked = 0;
		node->Slots[i].Type = NULL_CAPABILITY;
	}

	GenerateCapability(space, CAPABILITY_NODE, VMM::VirtualToPhysical(slabNodeFrame), ACCESS);
	slabNodeFrame += PAGE_SIZE;

	/* Now put all the capabilites in the respective slabs, - obviously the frames we used */

	/* TODO REHAUL */
	for (MemoryHeader *entry = memoryMap; entry->Address != (uptr)-1; ++entry) {
		switch(entry->Flags) {
			case MEMMAP_USABLE: {
				if(framesBase == entry->Address) {
					GenerateCapability(space, UNTYPED_FRAMES, VMM::VirtualToPhysical(slabNodeFrame), entry->Length - (VMM::VirtualToPhysical(slabNodeFrame) - entry->Address), ACCESS | RETYPE | GRANT);
				} else {
					GenerateCapability(space, UNTYPED_FRAMES, entry->Address, entry->Length, ACCESS | RETYPE | GRANT);
				}
				}
				break;
			case MEMMAP_RESERVED:
			case MEMMAP_ACPI_RECLAIMABLE:
				//GenerateCapability(space, UNTYPED_FRAMES, entry->Address, entry->Length, ACCESS);
				break;
//			case MEMMAP_FRAMEBUFFER:
//				for (usize i = 0,
//				GenerateCapability(space, MMIO_MEMORY, entry->Address, entry->Length, ACCESS | GRANT);
				break;
			case MEMMAP_ACPI_NVS:
			case MEMMAP_BAD_MEMORY:
				GenerateCapability(space, UNTYPED_DMA, entry->Address, entry->Length, ACCESS);
				break;
			case MEMMAP_KERNEL_AND_MODULES:
			case MEMMAP_BOOTLOADER_RECLAIMABLE:
				GenerateCapability(space, UNTYPED_FRAMES, entry->Address, entry->Length, ACCESS | READ);
				break;

		}

	}
	
	GenerateCapability(space, CONTAINER, VMM::VirtualToPhysical((uptr)info->RootContainer), ACCESS);
	GenerateCapability(space, CAPABILITY_SPACE, VMM::VirtualToPhysical((uptr)info->RootCSpace), ACCESS);
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");

	return VMM::VirtualToPhysical(slabNodeFrame);
}

usize GetObjectSize(OBJECT_TYPE kind) {
	switch(kind) {
		case UNTYPED_FRAMES:
			return -1;
		case UNTYPED_DMA:
			return -1;
		case FRAME_MEMORY:
			return PAGE_SIZE;
		case MMIO_MEMORY:
			return PAGE_SIZE;
		case VIRTUAL_MEMORY_PAGING_STRUCTURE:
			return PAGE_SIZE;
		case CAPABILITY_SPACE:
			return sizeof(CapabilitySpace);
		case CAPABILITY_NODE:
			return sizeof(CapabilityNode);
		case VIRTUAL_CPU:
			return sizeof(VirtualCPU);
		case CONTAINER:
			return sizeof(Container);
		case CONTEXT:
			return sizeof(SchedulerContext);
		default:
			return 0;
	}
}

Capability *GetUntyped(CapabilitySpace *space, usize idx) {
	CapabilitySlab *slab = &space->Slabs;
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	if(node == NULL) {
		return NULL;
	}

	while(node) {
		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			Capability *capability = &node->Slots[i];
			if (capability->IsMasked || capability->Type != UNTYPED_FRAMES) continue;
			else if (idx-- == 0) return capability;
		}

		node = (CapabilityNode*)node->Next;
	}

	node = (CapabilityNode*)slab->FullSlabs.Head;

	if(node == NULL) {
		return NULL;
	}

	while(node) {
		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			Capability *capability = &node->Slots[i];
			if (capability->IsMasked || capability->Type != UNTYPED_FRAMES) continue;
			else if (idx-- == 0) return capability;
		}

		node = (CapabilityNode*)node->Next;
	}

	return NULL;
}

Capability *AddressFirstCapability(CapabilitySpace *space, uptr ptr) {
	return (Capability*)SLAB::SearchClose(space->CapabilityTree, ptr);
}

Capability *AddressCapability(CapabilitySpace *space, uptr ptr) {
	return (Capability*)SLAB::Search(space->CapabilityTree, ptr);
}

Capability *AddressIOCapability(CapabilitySpace *space, uptr ptr) {
	return (Capability*)SLAB::Search(space->IOCapabilityTree, ptr);
}

Capability *AddressCPUCapability(CapabilitySpace *space, uptr ptr) {
	return (Capability*)SLAB::Search(space->CPUCapabilityTree, ptr);
}

Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, u16 accessRights) {
	return GenerateCapability(space, kind, object, GetObjectSize(kind), accessRights);
}

Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, usize size, u16 accessRights) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs;
	CapabilityTreeNode *capability = SLAB::AllocateSlabSlot(slab);
	if (capability == NULL) {
		return NULL;
	}

	capability->IsMasked = 0;
	capability->Type = kind;
	capability->Object = object;
	capability->Size = size;
	capability->AccessRights = accessRights;
	capability->AccessRightsMask = 0xFFFF;
	
	space->CapabilityTree = SLAB::Insert(space->CapabilityTree, capability);

	//PRINTK::PrintK(PRINTK_DEBUG "Generated capability of 0x%x (%d) with kind: %d\r\n", object, size, kind);

	return capability;
}

Capability *GenerateIOCapability(CapabilitySpace *space, uptr iostart, usize size, u16 accessRights) {
	CapabilitySlab *slab = &space->Slabs;
	CapabilityTreeNode *capability = SLAB::AllocateSlabSlot(slab);
	if (capability == NULL) {
		return NULL;
	}

	capability->IsMasked = 0;
	capability->Type = PORTIO_MEMORY;
	capability->IOStart = iostart;
	capability->Size = size;
	capability->AccessRights = accessRights;
	capability->AccessRightsMask = 0xFFFF;
	
	space->IOCapabilityTree = SLAB::Insert(space->IOCapabilityTree, capability);

	//PRINTK::PrintK(PRINTK_DEBUG "Generated IO capability of 0x%x (%d)\r\n", iostart, size);

	return capability;
}

Capability *GenerateCPUCapability(CapabilitySpace *space, usize timesliceStart, usize size, u16 accessRights) {
	CapabilitySlab *slab = &space->Slabs;
	CapabilityTreeNode *capability = SLAB::AllocateSlabSlot(slab);
	if (capability == NULL) {
		return NULL;
	}

	capability->IsMasked = 0;
	capability->Type = CPU_TIMESLICE;
	capability->TimesliceStart = timesliceStart;
	capability->Size = size;
	capability->AccessRights = accessRights;
	capability->AccessRightsMask = 0xFFFF;
	
	space->CPUCapabilityTree = SLAB::Insert(space->CPUCapabilityTree, capability);

	PRINTK::PrintK(PRINTK_DEBUG "Generated CPU capability of 0x%x (%d)\r\n", timesliceStart, size);

	return capability;
}


Capability *RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind, usize count, Capability **array) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT || kind == UNTYPED_FRAMES || kind == UNTYPED_DMA) {
		return NULL;
	}

	if (untyped->IsMasked != 0) {
		/* The ut capability must not have children */
		PRINTK::PrintK(PRINTK_DEBUG "Capability has children\r\n");
		return NULL;
	}

	u16 maskedRights = untyped->AccessRights & untyped->AccessRightsMask;
	if ((maskedRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		PRINTK::PrintK(PRINTK_DEBUG "No rights\r\n");
		return NULL;
	}
	
	uptr startAddress = untyped->Object;

	/* Don't worry about this, all structures will be aligned to be powers of two 
	 * to result in a null quotient always
	 */
	usize objectSize = GetObjectSize(kind);
	usize realCount = untyped->Size / objectSize;

	if (untyped->Size == 0) {
		PRINTK::PrintK(PRINTK_DEBUG "UT is dead\r\n");
		return NULL;
	}

	// TODO: Delete the untyped
	untyped->IsMasked = 1;
	SLAB::FreeSlabSlot(&space->Slabs, (CapabilityTreeNode*)untyped);
	
	for (usize i = 0; i < realCount; ++i) {
		Capability *cap = GenerateCapability(space, kind, startAddress + i * objectSize, objectSize, maskedRights);
		if (cap == NULL) {
			return NULL;
		}

		if (i < count) {
			array[i] = cap;
		}
	}
		
	return array[0];
}

Capability *UntypeObject(CapabilitySpace *space, Capability *capability) {
	if (!capability || capability->Type == UNTYPED_FRAMES || capability->Type == UNTYPED_DMA) {
		return NULL;
	}

	// TODO: Add more
	capability->Type = UNTYPED_FRAMES;

	(void)space;

	return capability;
}

Capability *SplitUntyped(CapabilitySpace *space, Capability *untyped, usize splitSize, usize count, Capability **array) {
	return SplitUntyped(space, untyped, splitSize, 0, count, array);
}

Capability *SplitUntyped(CapabilitySpace *space, Capability *untyped, usize splitSize, usize offset, usize count, Capability **array) {
	//PRINTK::PrintK(PRINTK_DEBUG "Splitting untyped of size %d\r\n", untyped->Size);

	if (untyped->IsMasked != 0) {
		PRINTK::PrintK(PRINTK_DEBUG "Capability has children\r\n");
		/* The ut capability must not have children */
		return NULL;
	}

	u16 maskedRights = untyped->AccessRights & untyped->AccessRightsMask;
	if ((maskedRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		return NULL;
	}

	if (count == 0) {
		return NULL;
	}

	usize totalSplitSize = splitSize * count;
	if (untyped->Size < totalSplitSize + offset) {
		/* We can't split it, it's too small */
		PRINTK::PrintK(PRINTK_DEBUG "Excessively big split amount: %d vs %d\r\n", untyped->Size, totalSplitSize);
		return NULL;
	}

	uptr initialAddress = untyped->Object;
	usize initialLength = untyped->Size;
	u16 rights = untyped->AccessRights;
	u16 mask = untyped->AccessRightsMask;

	/*
	array[0] = untyped;
	untyped->Size = splitSize;
	*/

	untyped->IsMasked = true;
	SLAB::FreeSlabSlot(&space->Slabs, (CapabilityTreeNode*)untyped);


	if (offset > 0) {
		GenerateCapability(space, UNTYPED_FRAMES, initialAddress, offset, rights);
	}

	for (usize i = 0; i < count; ++i) {
		Capability *cap = NULL;
	
		cap = GenerateCapability(space, UNTYPED_FRAMES, offset + initialAddress + splitSize * i, splitSize, rights);
		cap->AccessRightsMask = mask; 
		array[i] = cap;
	}
	

	if (initialLength > totalSplitSize) {
		Capability *last = GenerateCapability(space, UNTYPED_FRAMES, offset + initialAddress + totalSplitSize, initialLength - totalSplitSize - offset, rights);
		last->AccessRightsMask = mask;

		return last;
	}

	return array[count - 1];
}

Capability *MergeUntyped(CapabilitySpace *space, Capability *ut, Capability *other) {
	/* Merges adiacent untyped memory regions */
	if (ut->Object + ut->Size != other->Object) return NULL;

	ut->Size += other->Size;

	CapabilitySlab *slab = &space->Slabs;
	CapabilityTreeNode *node = SLAB::Search(space->CapabilityTree, other->Object);
	SLAB::FreeSlabSlot(slab, node);
	space->CapabilityTree = SLAB::Delete(space->CapabilityTree, node);

	return NULL;
}

usize GetFreeSlots(CapabilitySpace *space) {
	CapabilitySlab *slab = &space->Slabs;
	
	return SLAB::GetFreeSlabSlots(slab);	
}
	
void AddSlabNode(CapabilitySpace *space, Capability *capability) {
	if (!capability || capability->Type != CAPABILITY_NODE) {
		return;
	}
	
	CapabilitySlab *slab = &space->Slabs;

	capability->IsMasked = 1;

	CapabilityNode *node = (CapabilityNode*)VMM::PhysicalToVirtual(capability->Object);
	SLAB::AddSlabNode(slab, node);

	//PRINTK::PrintK(PRINTK_DEBUG "Added slab node 0x%x from cap 0x%x\r\n",
	//		capability->Object, capability);
}

void DumpCapabilitySlab(CapabilitySpace *space) {
	CapabilitySlab *slab = &space->Slabs;
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	if(node == NULL) {
		return;
	}

	while(node) {
		PRINTK::PrintK(PRINTK_DEBUG "CSlab\r\n");
		PRINTK::PrintK(PRINTK_DEBUG "Slots:\r\n");

		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			Capability *capability = &node->Slots[i];
			if (capability->Type == NULL_CAPABILITY) continue;
			PRINTK::PrintK(PRINTK_DEBUG "Slot #%d\r\n"
					            " Capability 0x%x\r\n"
						    "  Type:     %d\r\n"
						    "  Object:   0x%x\r\n", i, capability, capability->Type, capability->Object);
		}

		node = (CapabilityNode*)node->Next;
	}

	node = (CapabilityNode*)slab->FullSlabs.Head;

	if(node == NULL) {
		return;
	}

	while(node) {
		PRINTK::PrintK(PRINTK_DEBUG "CSlab\r\n");
		PRINTK::PrintK(PRINTK_DEBUG "Slots:\r\n");

		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			Capability *capability = &node->Slots[i];
			if (capability->Type == NULL_CAPABILITY) continue;
			PRINTK::PrintK(PRINTK_DEBUG "Slot #%d\r\n"
					            " Capability 0x%x\r\n"
						    "  Type:     %d\r\n"
						    "  Object:   0x%x\r\n", i, capability, capability->Type, capability->Object);
		}

		node = (CapabilityNode*)node->Next;
	}

}
}
