#pragma once
#include "cdefs.h"
#include <object.hpp>

/*
inline static void SetCap(Capability *capability, u8 type, uptr object, u16 accessRightsMask, Capability *next, Capability *prev, Capability *parent) {
	capability->IsMasked = 0;
	capability->Type = type;
	capability->Object = object;
	capability->AccessRightsMask = accessRightsMask;
	capability->Next = next;
	capability->Prev = prev;
	capability->Parent = parent;

}*/

namespace CAPABILITY {
	void InitializeRootSpace(uptr framesBase, UntypedHeader *memoryMap);
	usize GetObjectSize(OBJECT_TYPE kind);

	Capability *GenerateCapability(CapabilitySpace *space, OBJECT_TYPE kind, uptr object, u16 accessRights);
	Capability *RetypeUntyped(CapabilitySpace *space, Capability *untyped, OBJECT_TYPE kind);
	Capability *UntypeObject(CapabilitySpace *space, Capability *capability);
	Capability *SplitUntyped(Capability *untyped, usize splitSize, usize count);
	void MergeUntyped(CapabilitySpace *space, Capability *capability);

	void DumpCapabilitySlab(CapabilitySpace *space, OBJECT_TYPE kind);
}
