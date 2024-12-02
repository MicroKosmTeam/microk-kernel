#pragma once
#include "cdefs.h"
#include <object.hpp>

namespace CAPABILITY {
	uptr InitializeRootSpace(uptr framesBase, MemoryHeader *memoryMap);
	usize GetObjectSize(OBJECT_TYPE kind);

	Capability *AddressCapability(CapabilitySpace *space, uptr ptr, OBJECT_TYPE kind);
	Capability *AddressFirstCapability(CapabilitySpace *space, uptr ptr, OBJECT_TYPE kind);
	Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, u16 accessRights);
	Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, usize size, u16 accessRights);
	Capability *RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind, usize count, Capability **array);
	Capability *UntypeObject(CapabilitySpace *space, Capability *capability);
	Capability *SplitUntyped(CapabilitySpace *space, Capability *untyped, usize splitSize, usize count, Capability **array);
	Capability *SplitUntyped(CapabilitySpace *space, Capability *untyped, usize splitSize, usize offset, usize count, Capability **array);
	Capability *MergeUntyped(CapabilitySpace *space, Capability *capability);

	usize GetFreeSlots(CapabilitySpace *space, OBJECT_TYPE kind);
	void AddSlabNode(CapabilitySpace *space, OBJECT_TYPE kind, Capability *capability);

	void DumpCapabilitySlab(CapabilitySpace *space, OBJECT_TYPE kind);
}
