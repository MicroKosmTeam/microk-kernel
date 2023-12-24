#include <capability.hpp>
#include <memory.hpp>
#include <pmm.hpp>
#include <kinfo.hpp>

namespace CAPABILITY {
void InitializeRootSpace() {
	KInfo *info = GetInfo();

	info->CapabilityNodeSize = PAGE_SIZE;

	CapabilityNode *rootNode = (CapabilityNode*)VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	Memclr(rootNode, PAGE_SIZE);

	AddElementToList(rootNode, &info->RootCapabilitySpace.CapabilityNodeList);

	Capability *rootSpace = &rootNode->Slots[0];
	rootSpace->Type = ObjectType::NULL_CAPABILITY;
	rootSpace->Object = (uptr)rootNode;
	rootSpace->AccessRights.Read = 1;
	rootSpace->AccessRights.Grant = 1;


}
}
