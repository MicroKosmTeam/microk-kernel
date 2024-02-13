#pragma once
#include <object.hpp>

namespace CAPABILITY {
	/* Function used to initialze the capability infrastructure */
	void InitializeRootSpace();

	/* Function used to create the root cnode for each TCB */
	int CreateRootCNode(TaskControlBlock *tcb, CapabilitySpace *cspace);

	/* Function used to create normal cnodes */
	CapabilityNode *CreateCNode(CapabilitySpace *cspace, uptr addr, usize sizeBits);



	Capability *Originate(CapabilitySpace *space, uptr object, usize size, ObjectType type, u32 accessRights);
	Capability *Originate(CapabilitySpace *space, CapabilityNode *node, uptr object, usize size, ObjectType type, u32 accessRights);
	Capability *Originate(CapabilitySpace *space, CapabilityNode *node, usize slot, uptr object, usize size, ObjectType type, u32 accessRights);

	Capability *Retype(Capability *capability, ObjectType type, usize quantity);
	void Revoke(Capability *capability);

	int IsNodeInSpace(CapabilitySpace *space, CapabilityNode *node);
}
