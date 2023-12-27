#pragma once
#include <object.hpp>

namespace CAPABILITY {
	void InitializeRootSpace();
	
	Capability *OriginateCapability(CapabilitySpace *space, uptr object, usize size, ObjectType type, u32 accessRights);
	void RevokeCapability(Capability *capability);

	CapabilityNode *CreateCapabilityNode(CapabilitySpace *space);
}
