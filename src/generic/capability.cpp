#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <task.hpp>

namespace CAPABILITY {
void InitializeRootSpace() {
	KInfo *info = GetInfo();

	/* Getting the page for the TCB and the cspace */
	uptr tcbFrame = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	uptr cspaceFrame = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());

	/* Initializing the TCB and creating the root CSpace */
	info->RootTCB = TASK::InitializeTCB(tcbFrame);
	info->RootCSpace = (CapabilitySpace*)cspaceFrame;
	Memclr(cspaceFrame, PAGE_SIZE);

	/* Creating the root CNode for the */
	CreateRootCNode(info->RootTCB, info->RootCSpace);
	
	PRINTK::PrintK(PRINTK_DEBUG "Root space initialized.\r\n");
}

int CreateRootCNode(TaskControlBlock *tcb, CapabilitySpace *cspace) {
	KInfo *info = GetInfo();

	/* Getting the memory layout ready
	 *  _____________________
	 * | tcb | cnode | empty |
	 * |_____|_______|_______|
	 */
	uptr tcbFrame = (uptr)tcb;
	uptr cnodeFrame = cspaceFrame + sizeof(ThreadControlBlock);
	ROUND_UP_TO(cnodeFrame, 16);

	/* Calculate the size bits of the root cnode by seeing how many
	 * slots we need and rounding it up to the closes power of two
	 */
	usize neededSlots = TCBCNodeSlots::SLOT_COUNT;
	neededSlots = MATH::UpperPowerOfTwoUSIZE(neededSlots);
	usize cnodeSizeBits = MATH::GetPowerOfTwo(neededSlots);

	/* Assign the frames in the TCB */
	tcb->RootCNode = (CapabilityNode*)cnodeFrame;

	/* Make all the slots reserved in the root cnode */
	Memset(cnodeFrame, 0xff, sizeof(Capability) * neededSlots);

	/* Filling the fixed slots */

	/* Creating the CSpace capability */
	Originate(cspace,
	          tcb->RootCNode,
		  RootCNodeSlots::CSPACE_SLOT,
		  (uptr)cspace,
		  sizeof(CapabilitySpace),
		  ObjectType::CSPACE,
		  CapabilityRights::ACCESS);

	/* Creating the recursive Root CNode capability */
	Originate(cspace,
		  tcb->RootCNode,
		  RootCNodeSlots::ROOT_CNODE_SLOT,
		  (uptr)tcb->RootCNode,
		  neededSlots * sizeof(Capability),
		  ObjectType::CNODE,
		  CapabilityRights::ACCESS);

	/* Creating the TCB capability */
	Originate(cspace,
	          tcb->RootCNode,
		  RootCNodeSlots::TASK_CONTROL_BLOCK_SLOT,
		  tcbFrame,
		  sizeof(ThreadControlBlock),
		  ObjectType::TASK_CONTROL_BLOCK,
		  CapabilityRights::ACCESS);

	/* Adding the CNode to the CSpace */	
	AddElementToList(tcb->RootCNode, cspace->CapabilityNodeList);

	return 0;
}

CapabilityNode *CreateCNode(CapabilitySpace *cspace, uptr addr, usize sizeBits) {
	KInfo *info = GetInfo();

	/* Clearing the memory area */
	CapabilityNode *node = (CapabilityNode*)frame;
	Memclr(node, info->CapabilityNodeSize);

	/* Creating the recursive CNode capability */
	Originate(cspace,
		  node,
		  frame,
		  sizeBits * sizeof(Capability),
		  ObjectType::CNODE,
		  CapabilityRights::ACCESS);

	/* adding the CNode to the CSpace */
	AddElementToList(node, &space->CapabilityNodeList);

	return node;
}




	
Capability *Originate(CapabilitySpace *space, uptr object, usize size, ObjectType type, u32 accessRights) {
	KInfo *info = GetInfo();

	CapabilityNode *node = (CapabilityNode*)space->CapabilityNodeList.Tail;

	bool found = false;
	Capability *capability;

	while (!found && node != NULL) {
		capability = (Capability*)&node->Slots[0];
	
		for (usize size = 0; size < info->CapabilityNodeSize; size += sizeof(Capability), ++capability) {
			if (capability->Type == ObjectType::NULL_CAPABILITY) {
				found = true;
				break;
			}
		}

		node = (CapabilityNode*)node->Previous;
	}

	if (!found) {
		/* If it's not found, then someone needs to retype some memory in a cnode */
		return NULL;
	}

	capability->Type = type;
	capability->Object = object;
	capability->Size = size;
	capability->AccessRights = accessRights;

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x (0x%x bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Size, capability->Type, capability->AccessRights);

	return capability;
}

Capability *Originate(CapabilitySpace *space, CapabilityNode *node, uptr object, usize size, ObjectType type, u32 accessRights) {
	KInfo *info = GetInfo();
	(void)space;

	bool found = false;
	Capability *capability;

	capability = (Capability*)&node->Slots[0];
	
	for (usize size = 0; size < info->CapabilityNodeSize; size += sizeof(Capability), ++capability) {
		if (capability->Type == ObjectType::NULL_CAPABILITY) {
			found = true;
			break;
		}
	}

	if (!found) {
		/* If it's not found, then someone needs to retype some memory in a cnode */
		return NULL;
	}

	capability->Type = type;
	capability->Object = object;
	capability->Size = size;
	capability->AccessRights = accessRights;

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x (0x%x bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Size, capability->Type, capability->AccessRights);

	return capability;
}
	
Capability *Originate(CapabilitySpace *space, CapabilityNode *node, usize slot, uptr object, usize size, ObjectType type, u32 accessRights) {
	Capability *capability = (Capability*)&node->Slots[slot];
	(void)space;

	capability->Type = type;
	capability->Object = object;
	capability->Size = size;
	capability->AccessRights = accessRights;

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x (0x%x bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Size, capability->Type, capability->AccessRights);

	return capability;

}

Capability *Retype(CapabilitySpace *space, Capability *capability, ObjectType type, usize quantity) {
	KInfo *info = GetInfo();

	if (capability->Type != ObjectType::UNTYPED ||
	    (capability->AccessRights & CapabilityRights::RETYPE) == 0 ||
	    quantity == 0) {
		return NULL;
	}

	usize requiredObjectSize;
	Capability *newCapability = NULL;

	switch (type) {
		case ObjectType::CNODE: {
			requiredObjectSize = PAGE_SIZE * quantity;

			if (capability->Size > requiredObjectSize) {
				/* Create all the CNodes and assing their respective
				 * capabilites to their first slots
				 */
				uptr address = VMM::PhysicalToVirtual(capability->Object);
				CapabilityNode *node;

				/* Continue generating CNodes and their respective capabilities,
				 * decreasing the size of the remaining untyped memory
				 */
				do {
					node = CreateCNode(space, address);
					newCapability = Originate(space, node, (uptr)node, info->CapabilityNodeSize, type, capability->AccessRights);
					address += info->CapabilityNodeSize;
				} while(--quantity);
			} else if (capability->Size == requiredObjectSize) {
				/* Create all the CNodes and assing their respective
				 * capabilites to their first slots, but replacing
				 * the original capability with the first CNode capability
				 */

				uptr address = VMM::PhysicalToVirtual(capability->Object);
				CapabilityNode *node;
				
				/* First CNode */
				node = CreateCNode(space, address);

				/* Substitute the old capability */
				newCapability = capability;
				capability->Type = type;
				capability->Object = (uptr)node;
				capability->Size = PAGE_SIZE;
				/* Access rights are the same */

				/* Generate the new nodes.
				 * Note that if quantity == 1, this while
				 * will never be executed
				 */
				while (--quantity) {
					node = CreateCNode(space, address);
					newCapability = Originate(space, node, (uptr)node, PAGE_SIZE, type, capability->AccessRights);
					address += PAGE_SIZE;
				}
			} else {
				/* Insufficent size */
				newCapability = NULL;
			}


			}
			break;
		default:
			break;

	}

	return newCapability;
}

void Revoke(Capability *capability) {
	if (capability->Type == ObjectType::NULL_CAPABILITY ||
	    (capability->AccessRights & CapabilityRights::REVOKE) == 0) {
		return;
	}

	return;
}
	
int IsNodeInSpace(CapabilitySpace *space, CapabilityNode *node) {
	if (space == NULL) {
		return -ENOTPRESENT;
	}

	CapabilityNode *checkNode = (CapabilityNode*)space->CapabilityNodeList.Head;
	while(checkNode != NULL) {
		if (checkNode == node) {
			return 0;
		}

		node = (CapabilityNode*)node->Next;
	}

	return -ENOCAP;
}
}
