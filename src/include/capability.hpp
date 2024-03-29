#pragma once
#include "cdefs.h"
#include <object.hpp>

namespace CAPABILITY {
	/* Function used to initialze the capability infrastructure */
	void InitializeRootSpace();

	/* Function used to create the root cnode for each TCB */
	int CreateRootCNode(ThreadControlBlock *tcb, CapabilitySpace *cspace);

	/* Function used to create normal cnodes */
	CapabilityNode *CreateCNode(uptr addr, usize sizeBits);
	/**/
	CapabilityNode *CreateCNode(CapabilitySpace *cspace, uptr addr, usize sizeBits);
	/**/
	int AddCNode(CapabilitySpace *cspace, CapabilityNode *node);

	/* Function that checks whether a node is present in a CSpace */
	int IsNodeInSpace(CapabilitySpace *cspace, CapabilityNode *node);

	/**/
	int IsCapabilityInNode(CapabilityNode *node, usize nodeSlot);
	/**/
	int IsCapabilityInNode(CapabilityNode *node, Capability *capability);
	/**/
	usize GetCapabilitySlot(CapabilityNode *node, Capability *capability);

	/**/
	usize GetCapabilitySlot(CapabilityNode *node, Capability *capability);

	/* Function used to generate a capability and store it in a random
	 * slot in a cnode */
	Capability *Originate(CapabilityNode *node, uptr object, OBJECT_TYPE type, u32 accessRights);
	/* Function used to generate a capability and store it in a specific
	 * slot in a cnode */
	Capability *Originate(CapabilityNode *node, usize slot, uptr object, OBJECT_TYPE type, u32 accessRights);

	/**/
	Capability *Split(CapabilityNode *node, Capability *ut, usize splitSize);

	/**/
	Capability *Retype(CapabilityNode *node, Capability *ut, OBJECT_TYPE type, u32 accessRights);

}
