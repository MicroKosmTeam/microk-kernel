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

CapabilityNode *CreateCNode(CapabilitySpace *cspace, uptr addr, usize sizeBits) {
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

	/* adding the CNode to the CSpace */
	AddElementToList(cnode, &cspace->CapabilityNodeList);

	return cnode;
}

Capability *Originate(CapabilityNode *node, uptr object, OBJECT_TYPE type, u32 accessRights) {
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

			/* Print a debug message */
			PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Type, capability->AccessRights);

			/* Return the capability */
			return capability;
		}
	}

	/* No free slot found, return NULL */
	return NULL;
}
	
Capability *Originate(CapabilityNode *node, usize slot, uptr object, OBJECT_TYPE type, u32 accessRights) {
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

	/* Print a debug message */
	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Type, capability->AccessRights);

	/* Return the capability */
	return capability;
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
}
