#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <task.hpp>
#include <math.hpp>

namespace CAPABILITY {
void InitializeRootSpace(uptr framesBase) {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr tcbFrame = VMM::PhysicalToVirtual(framesBase);
	uptr cspaceFrame = tcbFrame + sizeof(TaskControlBlock);
	uptr slabNodeFrame = tcbFrame + PAGE_SIZE;

	/* Initializing the TCB and creating the root CSpace */
	info->RootTCB = TASK::InitializeTCB(tcbFrame);
	info->RootCSpace = (CapabilitySpace*)cspaceFrame;
	Memclr((void*)cspaceFrame, PAGE_SIZE);

	CapabiltySpace *space = info->RootCSpace;
	for (OBJECT_TYPE t = UNTYPED; t < OBJECT_TYPE_COUNT; ++t) {
		space->Slabs[t].NodesAvailable = true;

		CapabilityNode *node = (CapabilityNode*)slabNodeFrame;

		space->Slabs[t].CapabilityNodes.Head =
		space->Slabs[t].CapabilityNodes.Tail = node;

		node->Next = node->Previous = NULL;

		slabNodeFrame += PAGE_SIZE;
	}

	/* Now put all the capabilites in the respective slabs, - obviously the frames we used */
	// TODO
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");
}

usize GetObjectSize(OBJECT_TYPE kind) {
	switch(kind) {
		case UNTYPED:
			return 0;
		case FRAMES:
			return sizeof(CapabilityFrames);
		case CSPACE:
			return sizeof(CapabilitySpace);
		case CNODE:
			return sizeof(CapabilityNode);
		case DOMAIN:
			return sizeof(Domain);
		case SCHEDULER:
			return sizeof(Scheduler);
		case TASK_CONTROL_BLOCK:
			return sizeof(TaskControlBlock);
		case SCHEDULER_CONTEXT:
			return sizeof(SchedulerContext);
		case PAGING_STRUCTURE:
			return PAGE_SIZE;
		default:
			return -1;
	}
}

/**
 * CAPABILITY GENERATE:
 *	capability->IsMasked = 0;
			capability->Type = type;
			capability->Object = object;
			capability->AccessRights = accessRights;
			capability->AccessRightsMask = 0xFFFF;
			capability->Next =
			capability->Prev =
			capability->Parent = NULL;

 *
 */

Capability *RequestObject(CapabilitySpace *space, OBJECT_TYPE kind) {
	if (kind <= NULL_CAPABILITY || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];

	/* Here check in the relative slab if there is such an object, then return it
	 * If there is no object present, return NULL and tell the user to retype
	 */

}

void ReturnObject(CapabilitySpace *space, Capability *capability) {
	if (kind <= NULL_CAPABILITY || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];

	/* Object is unused now */

}

void RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind) {
	if (kind <= NULL_CAPABILITY || kind >= OBJECT_TYPE_COUNT || kind == UNTYPED) {
		return NULL;
	}

	CapabilitySlab *untypedSlab = &space->Slabs[UNTYPED];
	CapabilitySlab *slab = &space->Slabs[kind];
	
	if (untyped->IsMasked != 0) {
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
	usize count = header->Length / GetObjectSize(kind);


	/* Here check if there are enough slots in the slab, then allocate those slots and
	 * make the capabilities */
	// TODO

	/* From now on, the UntypedHeader is not to be read anymore, as it's considered overwritten */
	Memclr(header, sizeof(UntypedHeader));
}

void UntypeObject(CapabilitySpace *space, Capability *capability) {
	if (kind <= NULL_CAPABILITY || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];

	/* Transforms back into untyped */
}
	
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

void MergeUntyped(CapabilitySpace *space, Capability *capability) {
	if (kind <= NULL_CAPABILITY || kind >= OBJECT_TYPE_COUNT) {
		return NULL;
	}

	CapabilitySlab *slab = &space->Slabs[kind];

	/* Merges adiacent untyped memory regions */
}
/*
void DumpCNode(CapabilityNode *node) {
	if (node == NULL) {
		return;
	}

	usize size = MATH::ElevatePowerOfTwo(node->SizeBits);
		
	PRINTK::PrintK(PRINTK_DEBUG "CNode -> Size: 2^%d = %d bytes\r\n", node->SizeBits, size);
	PRINTK::PrintK(PRINTK_DEBUG "Slots:\r\n");

	usize slots = (size - sizeof(CapabilityNode)) / sizeof(Capability);

	for (usize i = 0; i < slots; ++i) {
		Capability *capability = &node->Slots[i];
		PRINTK::PrintK(PRINTK_DEBUG "Slot #%d\r\n"
				            " Capability 0x%x\r\n"
					    "  Type:     %d\r\n"
					    "  Object:   0x%x\r\n"
					    "  Next:     0x%x\r\n"
					    "  Previous: 0x%x\r\n", i, capability, capability->Type, capability->Object, capability->Next, capability->Prev);
	}
}
*/

}
