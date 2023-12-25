#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>
#include <printk.hpp>

namespace CAPABILITY {
void InitializeRootSpace() {
	KInfo *info = GetInfo();

	info->CapabilityNodeSize = PAGE_SIZE;

	CapabilityNode *rootNode = (CapabilityNode*)VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	Memclr(rootNode, PAGE_SIZE);

	AddElementToList(rootNode, &info->RootCapabilitySpace.CapabilityNodeList);

}
	
Capability *OriginateCapability(CapabilitySpace *space, uptr object, usize size, ObjectType type, u32 accessRights) {
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
		PRINTK::PrintK(PRINTK_DEBUG "New node to be originated.\r\n");
		node = (CapabilityNode*)VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
		Memclr(node, PAGE_SIZE);

		AddElementToList(node, &info->RootCapabilitySpace.CapabilityNodeList);
		capability = &node->Slots[0];
	}

	capability->Type = type;
	capability->Object = object;
	capability->Size = size;
	capability->AccessRights = accessRights;

	PRINTK::PrintK(PRINTK_DEBUG "Originated capability of object 0x%x (%d bytes) of type 0x%x and with the following access rights: 0x%x\r\n", capability->Object, capability->Size, capability->Type, capability->AccessRights);

	return capability;
}
	
void RevokeCapability(Capability *capability) {
	if (capability->Type == ObjectType::NULL_CAPABILITY ||
	    (capability->AccessRights & CapabilityRights::REVOKE) == 0) {
		return;
	}

	return;
}
}
