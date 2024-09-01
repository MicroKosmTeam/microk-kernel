#pragma once
#include <object.hpp>

namespace SLAB {
	CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node);
	CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node);
	CapabilityNode *FindSlabNode(CapabilitySlab *slab, uptr address);
	Capability *FindElementInNode(CapabilityNode *node, uptr address);
	Capability *FindElementInSlab(CapabilitySlab *slab, uptr address);
	
	Capability *AllocateFreeSlotInSlab(CapabilitySlab *slab);
}
