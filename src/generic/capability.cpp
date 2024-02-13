#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>
#include <task.hpp>

namespace CAPABILITY {
void InitializeRootSpace() {
	KInfo *info = GetInfo();

	uptr tcbFrame = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());

	info->RootTCB = TASK::InitializeTCB(tcbFrame);
	info->RootTCB->CSpace = (CapabilitySpace*)(tcbFrame + sizeof(ThreadControlBlock));
	info->RootCapabilitySpace = info->RootTCB->CSpace;
	info->CapabilityNodeSize = PAGE_SIZE;

	Memclr(info->RootCapabilitySpace, sizeof(CapabilitySpace));

	CapabilityNode *rootNode = CreateRootCNode(info->RootCapabilitySpace, VMM::PhysicalToVirtual((uptr)PMM::RequestPage()));
	Originate(info->RootCapabilitySpace, rootNode, RootCNodeSlots::TASK_CONTROL_BLOCK_SLOT, tcbFrame, sizeof(ThreadControlBlock), ObjectType::TASK_CONTROL_BLOCK, CapabilityRights::ACCESS | CapabilityRights::SEE);

	PRINTK::PrintK(PRINTK_DEBUG "Root CNode initialized.\r\n");
}

CapabilityNode *CreateCNode(CapabilitySpace *space, uptr frame) {
	KInfo *info = GetInfo();
	CapabilityNode *node = (CapabilityNode*)frame;
	Memclr(node, info->CapabilityNodeSize);

	Originate(space, node, (uptr)node, info->CapabilityNodeSize, ObjectType::CNODE, CapabilityRights::ACCESS);
	AddElementToList(node, &space->CapabilityNodeList);

	return node;
}

CapabilityNode *CreateRootCNode(CapabilitySpace *space, uptr frame) {
	KInfo *info = GetInfo();

	CapabilityNode *rootNode = (CapabilityNode*)frame;
	Memclr(rootNode, info->CapabilityNodeSize);

	for (usize slot = RootCNodeSlots::NULL_SLOT; slot < RootCNodeSlots::FIRST_FREE_SLOT; ++slot) {
		rootNode->Slots[slot].Type = ObjectType::RESERVED_SLOT;
	}

	Originate(space, rootNode, RootCNodeSlots::CSPACE_SLOT, (uptr)space, sizeof(CapabilitySpace), ObjectType::CSPACE, CapabilityRights::ACCESS);
	Originate(space, rootNode, RootCNodeSlots::ROOT_CNODE_SLOT, (uptr)rootNode, info->CapabilityNodeSize, ObjectType::CNODE, CapabilityRights::ACCESS);
	
	space->CapabilityNodeList.Head = space->CapabilityNodeList.Tail = rootNode;

	return rootNode;
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

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x (%d bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Size, capability->Type, capability->AccessRights);

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

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x (%d bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Size, capability->Type, capability->AccessRights);

	return capability;
}
	
Capability *Originate(CapabilitySpace *space, CapabilityNode *node, usize slot, uptr object, usize size, ObjectType type, u32 accessRights) {
	Capability *capability = (Capability*)&node->Slots[slot];
	(void)space;

	capability->Type = type;
	capability->Object = object;
	capability->Size = size;
	capability->AccessRights = accessRights;

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability 0x%x of object 0x%x (%d bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability, capability->Object, capability->Size, capability->Type, capability->AccessRights);

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
