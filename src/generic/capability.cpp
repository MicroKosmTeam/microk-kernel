#include <capability.hpp>
#include <memory.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <task.hpp>
#include <slab.hpp>
#include <math.hpp>

namespace CAPABILITY {
void InitializeRootSpace(uptr framesBase, UntypedHeader *memoryMap) {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr tcbFrame = VMM::PhysicalToVirtual(framesBase);
	uptr cspaceFrame = tcbFrame + sizeof(TaskControlBlock);
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
			node->Slots[i].IsClaimed = 0;
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
			UntypedHeader *accessibleHeader = (UntypedHeader*)VMM::PhysicalToVirtual(entry->Address);

			if(framesBase == entry->Address) {
				uptr oldAddress = entry->Address;
				entry->Address = slabNodeFrame;
				entry->Length -= (slabNodeFrame - oldAddress);
			} else {
				entry->Address = (uptr)accessibleHeader;
			}

			Memcpy(accessibleHeader, entry, sizeof(UntypedHeader));

			GenerateCapability(space, UNTYPED, (uptr)accessibleHeader->Address, ACCESS | RETYPE | GRANT);
		}

	}
	
	GenerateCapability(space, TASK_CONTROL_BLOCK, (uptr)info->RootTCB, ACCESS);
	GenerateCapability(space, CSPACE, (uptr)info->RootCSpace, ACCESS);
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");
	
	DumpCapabilitySlab(space, UNTYPED);
	DumpCapabilitySlab(space, TASK_CONTROL_BLOCK);
	DumpCapabilitySlab(space, CNODE);
	DumpCapabilitySlab(space, CSPACE);

	SLAB::Dump(space->CapabilityTree);
}

usize GetObjectSize(OBJECT_TYPE kind) {
	switch(kind) {
		case UNTYPED:
			return 0;
		case FRAMES:
			return sizeof(PAGE_SIZE);
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
			return sizeof(TaskControlBlock);
		case TASK_SCHEDULER_CONTEXT:
			return sizeof(SchedulerContext);
		default:
			return -1;
	}
}

Capability *AddressCapability(CapabilitySpace *space, uptr ptr) {
	OBJECT_TYPE kind = UNTYPED/*TODO*/;
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];

	(void)slab;
	(void)ptr;

	return NULL;
}


Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, u32 accessRights) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];
	CapabilityTreeNode *capability = SLAB::AllocateFreeSlotInSlab(slab);
	if (capability == NULL) {
		return NULL;
	}

	capability->IsMasked = 0;
	capability->Type = kind;
	capability->Object = object;
	capability->AccessRights = accessRights;
	capability->AccessRightsMask = 0xFFFF;
	
	space->CapabilityTree = SLAB::Insert(space->CapabilityTree, capability);

	PRINTK::PrintK(PRINTK_DEBUG "Generated capability of kind: %d\r\n", kind);

	return capability;
}

Capability *RequestObject(CapabilitySpace *space, OBJECT_TYPE kind) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];

	/* Here check in the relative slab if there is such an object, then return it
	 * If there is no object present, return NULL and tell the user to retype
	 */
	Capability *capability = SLAB::ClaimSlotInSlab(slab);

	return capability;
}

void ReturnObject(CapabilitySpace *space, Capability *capability) {
	/* Object is unused now */

	(void)space;
	capability->IsClaimed = false;
}

void RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind) {
	if (kind < UNTYPED || kind >= OBJECT_TYPE_COUNT || kind == UNTYPED) {
		return;
	}

	CapabilitySlab *untypedSlab = &space->Slabs[UNTYPED];
	CapabilitySlab *slab = &space->Slabs[kind];
	
	if (untyped->IsMasked != 0) {
		/* The ut capability must not have children */
		return;
	}

	u16 maskedRights = untyped->AccessRights & untyped->AccessRightsMask;
	if ((maskedRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		return;
	}
	
	UntypedHeader *header = (UntypedHeader*)untyped->Object;
	uptr startAddress = header->Address;

	/* Don't worry about this, all structures will be aligned to be powers of two 
	 * to result in a null quotient always
	 */
	usize count = header->Length / GetObjectSize(kind);


	/* Here check if there are enough slots in the slab, then allocate those slots and
	 * make the capabilities */
	// TODO
	(void)slab;
	(void)count;
	(void)startAddress;
	(void)untypedSlab;

	/* From now on, the UntypedHeader is not to be read anymore, as it's considered overwritten */
	Memclr(header, sizeof(UntypedHeader));
}

void UntypeObject(CapabilitySpace *space, Capability *capability) {
	CapabilitySlab *slab = &space->Slabs[UNTYPED];

	/* Transforms back into untyped */
	(void)slab;
	(void)capability;
}
#ifdef UNDEF	
Capability *SplitUntyped(Capability *untyped, usize splitSize, usize count) {
	if (untyped->IsMasked != 0) {
		/* The ut capability must not have children */
		return NULL;
	}


	u16 maskedRights = untyped->AccessRights & untyped->AccessRightsMask;
	if ((maskedRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		return NULL;
	}

	if (count == 0) {
		return NULL;
	} else if (count == 1) {
		return untyped;
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

	Capability *untypedNext = untyped->Next;
	Capability *untypedPrev= untyped->Prev;

	uptr initialAddress = header->Address;
	usize initialLength = header->Length;
	u16 rights = untyped->AccessRights;
	u16 mask = untyped->AccessRightsMask;
	uptr baseAddr = untyped->Object;

	Capability *next = NULL;
	Capability *prev = NULL;
	for (usize i = 1; i < count; ++i) {
		UntypedHeader *nextHeader = (UntypedHeader*)(baseAddr + splitSize * i);
		Memcpy(nextHeader, header, sizeof(UntypedHeader));
		nextHeader->Address = initialAddress + splitSize * i;
		nextHeader->Length = splitSize;
	
		if (next) {
			next = Originate(node, (uptr)nextHeader, OBJECT_TYPE::UNTYPED, rights);
		} else {
			next = untyped;
			next->Object = (uptr)nextHeader;
		}

		next->AccessRightsMask = mask; 

		next->Next = NULL;

		if (prev) {
			next->Prev = prev;
			prev->Next = next;
		} else {
			next->Prev = untypedPrev;
		}

		prev = next;
	}

	if (initialLength > totalSplitSize) {
		UntypedHeader *lastHeader = (UntypedHeader*)(untyped->Object + totalSplitSize);
		Memcpy(lastHeader, header, sizeof(UntypedHeader));
		lastHeader->Address = initialAddress + totalSplitSize;
		lastHeader->Length = initialLength - totalSplitSize;
	
		Capability *last = Originate(node, (uptr)lastHeader, OBJECT_TYPE::UNTYPED, rights);
		last->AccessRightsMask = mask;

		last->Next = untypedNext;

		if (untypedNext) {
			untypedNext->Prev = last;
		}

		last->Prev = prev;
		prev->Next = last;
	} else if (untypedNext) {
		prev->Next = untypedNext;
		untypedNext->Prev = prev;
	}
	

	return ut;
}
#endif
void MergeUntyped(CapabilitySpace *space, Capability *capability) {
	/* Merges adiacent untyped memory regions */
	(void)space;
	(void)capability;
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
