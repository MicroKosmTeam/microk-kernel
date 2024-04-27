#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <panic.hpp>
#include <task.hpp>
#include <math.hpp>

namespace CAPABILITY {
void InitializeRootSpace() {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr tcbFrame = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	uptr cspaceFrame = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());

	/* Initializing the TCB and creating the root CSpace */
	info->RootTCB = TASK::InitializeTCB(tcbFrame);
	info->RootCSpace = (CapabilitySpace*)cspaceFrame;
	Memclr((void*)cspaceFrame, PAGE_SIZE);

	/* Creating the root CNode for the */
	CreateRootCNode(info->RootTCB, info->RootCSpace);
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");
}

int CreateRootCNode(ThreadControlBlock *tcb, CapabilitySpace *cspace) {
	/* Getting the memory layout ready
	 *  _____________________
	 * | tcb | cnode | empty |
	 * |_____|_______|_______|
	 */
	uptr tcbFrame = (uptr)tcb;
	uptr cnodeFrame = tcbFrame + sizeof(ThreadControlBlock);
	ROUND_UP_TO(cnodeFrame, 16);

	/* Calculate the size bits of the root cnode by seeing how many
	 * slots we need and rounding it up to the closes power of two
	 */
	usize neededSlots = ROOT_CNODE_SLOTS::SLOT_COUNT;
	usize cnodeSize = neededSlots * sizeof(Capability) + sizeof(CapabilityNode);
	cnodeSize = MATH::UpperPowerOfTwoUSIZE(cnodeSize);
	usize cnodeSizeBits = MATH::GetPowerOfTwo(cnodeSize);

	/* Assign the frames in the TCB */
	tcb->RootCNode = (CapabilityNode*)cnodeFrame;

	/* Make all the slots reserved in the root cnode */
	Memset((void*)cnodeFrame, 0xff, cnodeSize);

	/* Setting the size bits */
	tcb->RootCNode->SizeBits = cnodeSizeBits;

	/* Filling the fixed slots */
	/* Creating the CSpace capability */
	Originate(tcb->RootCNode,
		  ROOT_CNODE_SLOTS::CSPACE_SLOT,
		  (uptr)cspace,
		  OBJECT_TYPE::CSPACE,
		  CAPABILITY_RIGHTS::ACCESS);

	/* Creating the recursive Root CNode capability */
	Originate(tcb->RootCNode,
		  ROOT_CNODE_SLOTS::ROOT_CNODE_SLOT,
		  (uptr)tcb->RootCNode,
		  OBJECT_TYPE::CNODE,
		  CAPABILITY_RIGHTS::ACCESS);

	/* Creating the TCB capability */
	Originate(tcb->RootCNode,
		  ROOT_CNODE_SLOTS::TASK_CONTROL_BLOCK_SLOT,
		  tcbFrame,
		  OBJECT_TYPE::TASK_CONTROL_BLOCK,
		  CAPABILITY_RIGHTS::ACCESS);

	/* Adding the CNode to the CSpace */	
	AddElementToList(tcb->RootCNode, &cspace->CapabilityNodeList);

	return 0;
}

CapabilityNode *CreateCNode(uptr addr, usize sizeBits) {
	/* By the way, the size has to be:
	 * (# of capabilities) x sizeof(Capability) + sizeof(CapabilityNode)
	 */
	CapabilityNode *cnode = (CapabilityNode*)addr;
	usize cnodeSize = MATH::ElevatePowerOfTwo(sizeBits);
	Memclr(cnode, cnodeSize);

	/* Setting the size bits */
	cnode->SizeBits = sizeBits;

	/* Creating the recursive CNode capability */
	Originate(cnode,
		  addr,
		  OBJECT_TYPE::CNODE,
		  CAPABILITY_RIGHTS::ACCESS);

	return cnode;
}


CapabilityNode *CreateCNode(CapabilitySpace *cspace, uptr addr, usize sizeBits) {
	CapabilityNode *cnode = CreateCNode(addr, sizeBits);
	AddCNode(cspace, cnode);

	return cnode;
}
	
int AddCNode(CapabilitySpace *cspace, CapabilityNode *node) {
	/* Adding the CNode to the CSpace */
	AddElementToList(node, &cspace->CapabilityNodeList);

	return 0;
}

int IsNodeInSpace(CapabilitySpace *cspace, CapabilityNode *node) {
	/* Check whether the cpsace exists */
	if (cspace == NULL) {
		return -ENOTPRESENT;
	}

	/* Run through the list of nodes and check if the addresses are the same */
	for (CapabilityNode *checkNode = (CapabilityNode*)cspace->CapabilityNodeList.Head;
	     checkNode != NULL;
	     node = (CapabilityNode*)node->Next) {
		if (checkNode == node) {
			/* If they're equal, then we've found it */
			return 0;
		}

	}

	/* The node hasn't been found anywhere in the cspace */
	return -ENOCAP;
}
	
int IsCapabilityInNode(CapabilityNode *node, usize nodeSlot) {
	if (node == NULL) {
		return -ENOTPRESENT;
	}

	usize nodeSize = MATH::ElevatePowerOfTwo(node->SizeBits);
	usize nodeSlots = nodeSize / sizeof(Capability);

	if (nodeSlots > nodeSlot) {
		return 0;
	}

	return -ENOCAP;
}

int IsCapabilityInNode(CapabilityNode *node, Capability *capability) {
	if (node == NULL || capability == NULL) {
		return -ENOTPRESENT;
	}

	usize nodeSize = MATH::ElevatePowerOfTwo(node->SizeBits);
	usize nodeSlots = nodeSize / sizeof(Capability);

	usize capabilitySlot = ((uptr)capability - (uptr)node - sizeof(CapabilityNode))  / sizeof(Capability);

	if (nodeSlots > capabilitySlot) {
		return 0;
	}

	return -ENOCAP;
}

usize GetCapabilitySlot(CapabilityNode *node, Capability *capability) {
	if (node == NULL || capability == NULL) {
		return -1;
	}

	uptr nodePtr = (uptr)node + sizeof(CapabilityNode);
	uptr capabilityPtr = (uptr)capability;

	usize capabilitySlot = (capabilityPtr - nodePtr)  / sizeof(Capability);

	return capabilitySlot;
}
	
CapabilityNode *ResolveCNode(Capability *nodeCap) {
	if (nodeCap == NULL) {
		return NULL;
	}

	if (nodeCap->Type != CNODE) {
		return NULL;
	}

	CapabilityNode *node = (CapabilityNode*)VMM::PhysicalToVirtual(nodeCap->Object);
	return node;
}

Capability *Originate(CapabilityNode *node, uptr object, OBJECT_TYPE type, u16 accessRights) {
	/* Cycle through the slots of the CNode to find the first free slot */
	for (Capability *capability = &node->Slots[0];
	     /* This checks whether the capability is within the CNode */
	     (uptr)capability < (uptr)node + MATH::ElevatePowerOfTwo(node->SizeBits);
	     ++capability) {
		/* If the slot is a null capability, it's free real estate */
		if (capability->Type == OBJECT_TYPE::NULL_CAPABILITY) {
			/* Assign the various paramenters to the capability */
			capability->Type = type;
			capability->Object = object;
			capability->AccessRights = accessRights;
			capability->Children = 0;
			capability->Parent = NULL;

			/* Print a debug message */
			PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Type, capability->AccessRights);

			/* Return the capability */
			return capability;
		}
	}

	/* No free slot found, return NULL */
	return NULL;
}
	
Capability *Originate(CapabilityNode *node, usize slot, uptr object, OBJECT_TYPE type, u16 accessRights) {
	/* Check whether the slot is actually valid */
	usize cnodeSlotsSize = MATH::ElevatePowerOfTwo(node->SizeBits) - sizeof(CapabilityNode);
	if (cnodeSlotsSize / sizeof(Capability) <= slot) {
		return NULL;
	}

	/* Get the capability slot in the cnode */
	Capability *capability = (Capability*)&node->Slots[slot];

	/* Assign the various paramenters to the capability */
	capability->Type = type;
	capability->Object = object;
	capability->AccessRights = accessRights;
	capability->Children = 0;
	capability->Parent = NULL;

	/* Print a debug message */
	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Type, capability->AccessRights);

	/* Return the capability */
	return capability;
}
	
Capability *Split(CapabilityNode *node, Capability *ut, usize splitSize, usize count) {
	if (node == NULL ||
	    ut == NULL) {
		return NULL;
	}

	if (ut->Children != 0) {
		/* The ut capability must not have children */
		return NULL;
	}

	if ((ut->AccessRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		return NULL;
	}

	if (count == 0) {
		return NULL;
	} else if (count == 1) {
		return ut;
	}

	/* Can't be split to less than a page */
	ROUND_UP_TO_PAGE(splitSize);

	UntypedHeader *header = (UntypedHeader*)ut->Object;

	usize totalSplitSize = splitSize * count;
	if (header->Length < totalSplitSize) {
		/* We can't split it, it's too small */
		return NULL;
	}

	ut->Children += count;

	Capability *first = Originate(node, (uptr)header, OBJECT_TYPE::UNTYPED, ut->AccessRights);
	first->Parent = ut;


	for (usize i = 0; i < count; ++i) {
		UntypedHeader *nextHeader = (UntypedHeader*)(ut->Object + splitSize * i);
		Memcpy(nextHeader, header, sizeof(UntypedHeader));
		nextHeader->Address += splitSize * i;
		nextHeader->Length = splitSize;
	
		Capability *next = Originate(node, (uptr)nextHeader, OBJECT_TYPE::UNTYPED, ut->AccessRights);
		next->Parent = ut;
	}

	if (header->Length > totalSplitSize) {
		UntypedHeader *lastHeader = (UntypedHeader*)(ut->Object + totalSplitSize);
		Memcpy(lastHeader, header, sizeof(UntypedHeader));
		lastHeader->Address += totalSplitSize;
		lastHeader->Length = header->Length - totalSplitSize;
	
		Capability *last= Originate(node, (uptr)lastHeader, OBJECT_TYPE::UNTYPED, ut->AccessRights);
		last->Parent = ut;
	}
		
	header->Length = splitSize;

	return first;
}
	
Capability *Retype(CapabilityNode *node, Capability *ut, OBJECT_TYPE type, u16 accessRights) {
	if (node == NULL ||
	    ut == NULL) {
		return NULL;
	}

	if (ut->Children != 0) {
		/* The ut capability must not have children */
		return NULL;
	}

	if ((ut->AccessRights & CAPABILITY_RIGHTS::RETYPE) == 0) {
		return NULL;
	}

	if (type <= OBJECT_TYPE::UNTYPED ||
	    type >= OBJECT_TYPE::OBJECT_TYPE_COUNT ) {
		/* The type must exist */
		return NULL;
	}
	
	UntypedHeader *header = (UntypedHeader*)ut->Object;

	usize length = header->Length;
	if (!MATH::IsPowerOfTwo(length)) {
		/* Must be a power of two */
		return NULL;
	}


	Capability *retyped = Originate(node, header->Address, type, accessRights);
	ut->Children += 1;
	retyped->Parent = ut;
	
	/* From now on, the UntypedHeader is not to be read anymore, as it's considered overwritten */
	Memclr(header, sizeof(UntypedHeader));

	if (type == OBJECT_TYPE::CNODE) {
		/* Special, needs to be assigned the size */
		usize sizeBits = MATH::GetPowerOfTwo(length);

		CAPABILITY::CreateCNode(ut->Object, sizeBits);
	}

	return retyped;
}
	
int Revoke(CapabilityNode *node, Capability *cap) {
	TODO;

	if (node == NULL ||
	    cap == NULL) {
		return -ENOCAP;
	}

	if (cap->Children != 0) {
		/* The ut capability must not have children */
		return -EBUSY;
	}

	if ((cap->AccessRights & CAPABILITY_RIGHTS::REVOKE) == 0) {
		return -EPERMISSION;
	}

	if (cap->Parent == NULL) {
		return -ENOCAP;
	}

	Capability *parent = cap->Parent;
	UntypedHeader *header = (UntypedHeader*)cap->Object;

	(void)parent, (void)header;

/*
	switch(cap->Type) {
		case OBJECT_TYPE::CNODE: {
			CapabilityNode *node = (CapabilityNode*)cap->Address;
			usize size = 
			Mw
			header->Address = cap->Address;

			if (parent->

	}
*/
	return ENOERR;
}

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
					    "  Type: %d\r\n"
					    "  Object: 0x%x\r\n", i, capability, capability->Type, capability->Object);
	}
}
}
