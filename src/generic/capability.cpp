#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>

namespace CAPABILITY {
void InitializeRootSpace() {
	KInfo *info = GetInfo();

	info->CapabilityNodeSize = PAGE_SIZE;
	info->RootCapabilitySpace = (CapabilitySpace*)VMM::PhysicalToVirtual((uptr)PMM::RequestPage());

	CapabilitySpace *rootSpace = info->RootCapabilitySpace;
	Memclr(rootSpace, PAGE_SIZE);

	CreateCNode(rootSpace, VMM::PhysicalToVirtual((uptr)PMM::RequestPage()));
	Originate(rootSpace, (uptr)rootSpace, sizeof(CapabilitySpace), ObjectType::CSPACE, CapabilityRights::NONE);
}

CapabilityNode *CreateCNode(CapabilitySpace *space, uptr frame) {
	CapabilityNode *node = (CapabilityNode*)frame;
	Memclr(node, PAGE_SIZE);

	Originate(space, node, (uptr)node, PAGE_SIZE, ObjectType::CNODE, CapabilityRights::REVOKE | CapabilityRights::GRANT | CapabilityRights::RETYPE);
	AddElementToList(node, &space->CapabilityNodeList);

	return node;
}

Capability *ResolvePointer(CapabilitySpace *space, CapabilityPointer ptr) {
	/* TODO, implement actual *SECURE* logic */
	(void)space;
	return (Capability*)ptr.Address;
}

CapabilityPointer GeneratePointer(Capability *capability) {
	/* TODO, implement actual *SECURE* logic */
	CapabilityPointer ptr;
	ptr.Address = (uptr)capability;
	return ptr;
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


Capability *Retype(CapabilitySpace *space, Capability *capability, ObjectType type, usize quantity) {
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
					newCapability = Originate(space, node, (uptr)node, PAGE_SIZE, type, capability->AccessRights);
					address += PAGE_SIZE;
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
}
