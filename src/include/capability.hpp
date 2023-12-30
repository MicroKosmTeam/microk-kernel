#pragma once
#include <object.hpp>

namespace CAPABILITY {
	void InitializeRootSpace();
	
	CapabilityNode *CreateCNode(CapabilitySpace *space, uptr addr);
	
	Capability *Originate(CapabilitySpace *space, uptr object, usize size, ObjectType type, u32 accessRights);
	Capability *Originate(CapabilitySpace *space, CapabilityNode *node, uptr object, usize size, ObjectType type, u32 accessRights);
	Capability *Retype(Capability *capability, ObjectType type, usize quantity);
	void Revoke(Capability *capability);

}
