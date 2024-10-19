#include <capability.hpp>
#include <memory.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <thread.hpp>
#include <slab.hpp>
#include <math.hpp>

namespace CAPABILITY {
uptr InitializeRootSpace(uptr framesBase, UntypedHeader *memoryMap) {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr tcbFrame = VMM::PhysicalToVirtual(framesBase);
	Memclr((void*)tcbFrame, PAGE_SIZE);
	uptr cspaceFrame = tcbFrame + sizeof(ThreadControlBlock);
	uptr slabNodeFrame = tcbFrame + PAGE_SIZE;

	/* Initializing the TCB and creating the root CSpace */
	info->RootTCB = TASK::InitializeTCB(tcbFrame);
	info->RootCSpace = (CapabilitySpace*)cspaceFrame;

	CapabilitySpace *space = info->RootCSpace;
	uptr nextSlabNodeFrame = slabNodeFrame;
	for (int t = UNTYPED; t < OBJECT_TYPE_COUNT; ++t) {
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

	for (int t = UNTYPED; t < OBJECT_TYPE_COUNT; ++t) {
		GenerateCapability(space, CNODE, slabNodeFrame, ACCESS);
		slabNodeFrame += PAGE_SIZE;
	}

	/* Now put all the capabilites in the respective slabs, - obviously the frames we used */

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

			GenerateCapability(space, UNTYPED, (uptr)accessibleHeader, ACCESS | RETYPE | GRANT);
		}

	}
	
	GenerateCapability(space, TASK_CONTROL_BLOCK, (uptr)info->RootTCB, ACCESS);
	GenerateCapability(space, CSPACE, (uptr)info->RootCSpace, ACCESS);
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");

	return slabNodeFrame;
}

usize GetObjectSize(OBJECT_TYPE kind) {
	switch(kind) {
		case UNTYPED:
			return 0;
		case FRAMES:
			return PAGE_SIZE;
		case VIRTUAL_MEMORY_MAPPING:
			return 0;
		case VIRTUAL_MEMORY_PAGING_STRUCTURE:
			return PAGE_SIZE;
		case CSPACE:
			return sizeof(CapabilitySpace);
		case CNODE:
			return sizeof(CapabilityNode);
		case CPU_DOMAIN:
			return sizeof(Domain);
		case PROCESS_SCHEDULER:
			return sizeof(Scheduler);
		case TASK_CONTROL_BLOCK:
			return sizeof(ThreadControlBlock);
		case TASK_SCHEDULER_CONTEXT:
			return sizeof(SchedulerContext);
		default:
			return -1;
	}
}

Capability *AddressCapability(CapabilitySpace *space, uptr ptr, OBJECT_TYPE kind) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}
	
	CapabilitySlab *slab = &space->Slabs[kind];

	return (Capability*)SLAB::Search(slab->CapabilityTree, ptr);
}


Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, u16 accessRights) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];
	CapabilityTreeNode *capability = SLAB::AllocateSlabSlot(slab);
	if (capability == NULL) {
		return NULL;
	}

	capability->IsMasked = 0;
	capability->Type = kind;
	capability->Object = object;
	capability->AccessRights = accessRights;
	capability->AccessRightsMask = 0xFFFF;
	
	//capability->Key = object;
	slab->CapabilityTree = SLAB::Insert(slab->CapabilityTree, capability);

	PRINTK::PrintK(PRINTK_DEBUG "Generated capability of 0x%x with kind: %d\r\n", object, kind);

	return capability;
}


Capability *RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind, usize count, Capability **array) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT || kind == UNTYPED) {
		return NULL;
	}

	if (untyped->IsMasked != 0 || untyped->Children != 0) {
		/* The ut capability must not have children */
		return NULL;
	}

	u16 maskedRights = untyped->AccessRights & untyped->AccessRightsMask;
	if ((maskedRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		return NULL;
	}
	
	UntypedHeader *header = (UntypedHeader*)untyped->Object;
	uptr startAddress = header->Address;

	/* Don't worry about this, all structures will be aligned to be powers of two 
	 * to result in a null quotient always
	 */
	usize objectSize = GetObjectSize(kind);
	usize realCount = header->Length / objectSize;

	/* Here check if there are enough slots in the slab, then allocate those slots and
	 * make the capabilities */
	for (usize i = 0; i < realCount; ++i) {
		Capability *cap = GenerateCapability(space, kind, startAddress + i * objectSize, maskedRights);
		if (cap == NULL) {
			return NULL;
		}

		cap->Parent = untyped;
		++untyped->Children;

		if (i < count) {
			array[i] = cap;
		}
	}
		
	untyped->IsMasked = 1;

	/* From now on, the UntypedHeader is not to be read anymore, as it's considered overwritten */
	Memclr(header, sizeof(UntypedHeader));

	return array[0];
}

Capability *UntypeObject(CapabilitySpace *space, Capability *capability) {
	CapabilitySlab *slab = &space->Slabs[UNTYPED];

	/* Transforms back into untyped */
	(void)slab;
	(void)capability;
	return NULL;
}

Capability *SplitUntyped(CapabilitySpace *space, Capability *untyped, usize splitSize, usize count, Capability **array) {
	if (untyped->IsMasked != 0 || untyped->Children != 0) {
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
		return NULL;
	}

	if (header->Length < totalSplitSize) {
		/* We can't split it, it's too small */
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
	
		cap = GenerateCapability(space, UNTYPED, (uptr)capHeader, rights);
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
	
		Capability *last = GenerateCapability(space, UNTYPED, (uptr)lastHeader, rights);
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
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
		return -1;
	}

	CapabilitySlab *slab = &space->Slabs[kind];
	
	return SLAB::GetFreeSlabSlots(slab);	
}
	
void AddSlabNode(CapabilitySpace *space, OBJECT_TYPE kind, Capability *capability) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT || !capability || capability->Type != CNODE) {
		return;
	}
	
	CapabilitySlab *slab = &space->Slabs[kind];

	capability->IsMasked = 1;

	CapabilityNode *node = (CapabilityNode*)capability->Object;
	SLAB::AddSlabNode(slab, node);

	PRINTK::PrintK(PRINTK_DEBUG "Added slab node 0x%x from cap 0x%x\r\n",
			capability->Object, capability);
}

void DumpCapabilitySlab(CapabilitySpace *space, OBJECT_TYPE kind) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
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
