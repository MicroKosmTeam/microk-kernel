#pragma once
#include <object.hpp>

namespace CAPABILITY {
	inline __attribute__((always_inline)) 
	CapabilityNode *GetRootNode(CapabilitySpace *space) {
		return (CapabilityNode*)space->CapabilityNodeList.Head;
	}

	void InitializeRootSpace();

	CapabilityNode *CreateCNode(CapabilitySpace *space, uptr addr);
	CapabilityNode *CreateRootCNode(CapabilitySpace *space, uptr frame);

	Capability *Originate(CapabilitySpace *space, uptr object, usize size, ObjectType type, u32 accessRights);
	Capability *Originate(CapabilitySpace *space, CapabilityNode *node, uptr object, usize size, ObjectType type, u32 accessRights);
	Capability *Originate(CapabilitySpace *space, CapabilityNode *node, usize slot, uptr object, usize size, ObjectType type, u32 accessRights);

	Capability *Retype(Capability *capability, ObjectType type, usize quantity);
	void Revoke(Capability *capability);

}
