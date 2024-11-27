#include <capability.hpp>
#include <memory.hpp>
#include <kinfo.hpp>
#include <container.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <slab.hpp>
#include <math.hpp>

namespace CAPABILITY {
uptr InitializeRootSpace(uptr framesBase, UntypedHeader *memoryMap) {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr containerFrame = VMM::PhysicalToVirtual(framesBase);
	Memclr((void*)containerFrame, PAGE_SIZE);
	uptr slabNodeFrame = containerFrame + PAGE_SIZE;

	/* Initializing the TCB and creating the root CSpace */
	info->RootContainer = CONTAINERS::InitializeContainer(containerFrame);
	info->RootCSpace = &info->RootContainer->CSpace;

	CapabilitySpace *space = info->RootCSpace;
	uptr nextSlabNodeFrame = slabNodeFrame;
	for (int t = UNTYPED_FRAMES; t < OBJECT_TYPE_COUNT; ++t) {
		CapabilityNode *node = (CapabilityNode*)nextSlabNodeFrame;

		space->Slabs[t].FreeSlabs.Head =
		space->Slabs[t].FreeSlabs.Tail = node;

		space->Slabs[t].UsedSlabs.Head =
		space->Slabs[t].UsedSlabs.Tail = NULL;

		space->Slabs[t].FullSlabs.Head =
		space->Slabs[t].FullSlabs.Tail = NULL;

		node->Next = node->Previous = NULL;
			
		node->FreeElements = CAPABILITIES_PER_NODE;

		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			node->Slots[i].IsMasked = 0;
			node->Slots[i].Type = NULL_CAPABILITY;
		}

		nextSlabNodeFrame += PAGE_SIZE;
	}

	for (int t = UNTYPED_FRAMES; t < OBJECT_TYPE_COUNT; ++t) {
		GenerateCapability(space, CAPABILITY_NODE, slabNodeFrame, ACCESS);
		slabNodeFrame += PAGE_SIZE;
	}

	/* Now put all the capabilites in the respective slabs, - obviously the frames we used */

	/* TODO REHAUL */
	for (UntypedHeader *entry = memoryMap; entry->Address != (uptr)-1; ++entry) {
		if(entry->Flags == MEMMAP_USABLE) {
			UntypedHeader *accessibleHeader;
			if(framesBase == entry->Address) {
				uptr oldAddress = entry->Address;
				
				accessibleHeader = (UntypedHeader*)slabNodeFrame;
				Memcpy(accessibleHeader, entry, sizeof(UntypedHeader));

				accessibleHeader->Address = slabNodeFrame;
				accessibleHeader->Length -= (accessibleHeader->Address - oldAddress);
			} else {
				accessibleHeader = (UntypedHeader*)VMM::PhysicalToVirtual(entry->Address);
			
				Memcpy(accessibleHeader, entry, sizeof(UntypedHeader));
			}

			GenerateCapability(space, UNTYPED_FRAMES, (uptr)accessibleHeader, ACCESS | RETYPE | GRANT);
		} else {
			/* LOGIC HERE */
			//GenerateCapability(space, UNTYPED_FRAMES_DMA, (uptr)accessibleHeader, ACCESS | RETYPE | GRANT);
		}

	}
	
	GenerateCapability(space, CONTAINER, (uptr)info->RootContainer, ACCESS);
	GenerateCapability(space, CAPABILITY_SPACE, (uptr)info->RootCSpace, ACCESS);
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");

	return slabNodeFrame;
}

usize GetObjectSize(OBJECT_TYPE kind) {
	switch(kind) {
		case UNTYPED_FRAMES:
			return -1;
		case UNTYPED_DMA:
			return -1;
		case FRAME_MEMORY:
			return PAGE_SIZE;
		case DMA_MEMORY:
			return -1;
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

Capability *AddressFirstCapability(CapabilitySpace *space, uptr ptr, OBJECT_TYPE kind) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}
	
	CapabilitySlab *slab = &space->Slabs[kind];

	return (Capability*)SLAB::SearchClose(slab->CapabilityTree, ptr);
}

Capability *AddressCapability(CapabilitySpace *space, uptr ptr, OBJECT_TYPE kind) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}
	
	CapabilitySlab *slab = &space->Slabs[kind];

	return (Capability*)SLAB::Search(slab->CapabilityTree, ptr);
}


Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, u16 accessRights) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];
	CapabilityTreeNode *capability = SLAB::AllocateSlabSlot(slab);
	if (capability == NULL) {
		return NULL;
	}

	capability->IsMasked = 0;
	capability->Type = kind;
	capability->Children = 0;
	capability->Object = object;
	capability->AccessRights = accessRights;
	capability->AccessRightsMask = 0xFFFF;
	
	//capability->Key = object;
	slab->CapabilityTree = SLAB::Insert(slab->CapabilityTree, capability);

	//PRINTK::PrintK(PRINTK_DEBUG "Generated capability of 0x%x with kind: %d\r\n", object, kind);

	return capability;
}


Capability *RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind, usize count, Capability **array) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT || kind == UNTYPED_FRAMES || kind == UNTYPED_DMA) {
		return NULL;
	}

	if (untyped->IsMasked != 0 || untyped->Children != 0) {
		/* The ut capability must not have children */
		PRINTK::PrintK(PRINTK_DEBUG "Capability has children\r\n");
		return NULL;
	}

	u16 maskedRights = untyped->AccessRights & untyped->AccessRightsMask;
	if ((maskedRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		PRINTK::PrintK(PRINTK_DEBUG "No rights\r\n");
		return NULL;
	}
	
	UntypedHeader *header = (UntypedHeader*)untyped->Object;
	uptr startAddress = header->Address;

	/* Don't worry about this, all structures will be aligned to be powers of two 
	 * to result in a null quotient always
	 */
	usize objectSize = GetObjectSize(kind);
	usize realCount = header->Length / objectSize;

	if (header->Length == 0) {
		PRINTK::PrintK(PRINTK_DEBUG "UT is dead\r\n");
		return NULL;
	}

	// TODO: Delete the untyped
	//CapabilitySlab *slab = &space->Slabs[UNTYPED_FRAMES];
	//CapabilityTreeNode *node = SLAB::Search(slab->CapabilityTree, untyped->Object);
	//SLAB::Delete(slab->CapabilityTree, node);

	for (usize i = 0; i < realCount; ++i) {
		Capability *cap = GenerateCapability(space, kind, startAddress + i * objectSize, maskedRights);
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
	CapabilitySlab *slab = &space->Slabs[UNTYPED_FRAMES];

	/* Transforms back into untyped */
	(void)slab;
	(void)capability;
	return NULL;
}

Capability *SplitUntyped(CapabilitySpace *space, Capability *untyped, usize splitSize, usize count, Capability **array) {
	if (untyped->IsMasked != 0 || untyped->Children != 0) {
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

	/* Can't be split to less than a page */
	ROUND_UP_TO_PAGE(splitSize);

	UntypedHeader *header = (UntypedHeader*)untyped->Object;

	usize totalSplitSize = splitSize * count;
	if (splitSize < sizeof(UntypedHeader)) {
		/* We can't split it, it's too small */
		PRINTK::PrintK(PRINTK_DEBUG "Excessively small split amount\r\n");
		return NULL;
	}

	if (header->Length == totalSplitSize) {
		/* We can't split it, it's too small */
		PRINTK::PrintK(PRINTK_DEBUG "Excessively big split amount: %d\r\n", totalSplitSize);
		array[0] = untyped;
		if (count > 1) {
			array[1] = NULL;
		}
		return untyped;
	}

	if (header->Length < totalSplitSize) {
		/* We can't split it, it's too small */
		PRINTK::PrintK(PRINTK_DEBUG "Excessively big split amount: %d vs %d\r\n", header->Length, totalSplitSize);
		return NULL;
	}

	uptr initialAddress = header->Address;
	usize initialLength = header->Length;
	u16 rights = untyped->AccessRights;
	u16 mask = untyped->AccessRightsMask;
	uptr baseAddr = untyped->Object;

	for (usize i = 1; i < count; ++i) {
		Capability *cap = NULL;
		UntypedHeader *capHeader = (UntypedHeader*)(baseAddr + splitSize * i);
		Memcpy(capHeader, header, sizeof(UntypedHeader));
		capHeader->Address = initialAddress + splitSize * i;
		capHeader->Length = splitSize;
	
		cap = GenerateCapability(space, UNTYPED_FRAMES, (uptr)capHeader, rights);
		cap->AccessRightsMask = mask; 
		array[i] = cap;
	}

	header->Length = splitSize;
	array[0] = untyped;

	if (initialLength > totalSplitSize) {
		UntypedHeader *lastHeader = (UntypedHeader*)(untyped->Object + totalSplitSize);
		Memcpy(lastHeader, header, sizeof(UntypedHeader));
		lastHeader->Address = initialAddress + totalSplitSize;
		lastHeader->Length = initialLength - totalSplitSize;
	
		Capability *last = GenerateCapability(space, UNTYPED_FRAMES, (uptr)lastHeader, rights);
		last->AccessRightsMask = mask;

		return last;
	}

	return array[count - 1];
}

Capability *MergeUntyped(CapabilitySpace *space, Capability *capability) {
	/* Merges adiacent untyped memory regions */
	(void)space;
	(void)capability;

	return NULL;
}

usize GetFreeSlots(CapabilitySpace *space, OBJECT_TYPE kind) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT) {
		return -1;
	}

	CapabilitySlab *slab = &space->Slabs[kind];
	
	return SLAB::GetFreeSlabSlots(slab);	
}
	
void AddSlabNode(CapabilitySpace *space, OBJECT_TYPE kind, Capability *capability) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT || !capability || capability->Type != CAPABILITY_NODE) {
		return;
	}
	
	CapabilitySlab *slab = &space->Slabs[kind];

	capability->IsMasked = 1;

	CapabilityNode *node = (CapabilityNode*)capability->Object;
	SLAB::AddSlabNode(slab, node);

	//PRINTK::PrintK(PRINTK_DEBUG "Added slab node 0x%x from cap 0x%x\r\n",
	//		capability->Object, capability);
}

void DumpCapabilitySlab(CapabilitySpace *space, OBJECT_TYPE kind) {
	if (kind < UNTYPED_FRAMES || kind >= OBJECT_TYPE_COUNT) {
		return;
	}

	CapabilitySlab *slab = &space->Slabs[kind];
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	if(node == NULL) {
		return;
	}

	PRINTK::PrintK(PRINTK_DEBUG "CSlab\r\n");
	PRINTK::PrintK(PRINTK_DEBUG "Slots:\r\n");

	while(node) {
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
