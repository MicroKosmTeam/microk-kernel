#pragma once
#include <object.hpp>

namespace SLAB {
	CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node);

	CapabilityTreeNode *AllocateSlabSlot(CapabilitySlab *slab);
	void FreeSlabSlot(CapabilitySlab *slab, CapabilityTreeNode *capability);
	usize GetFreeSlabSlots(CapabilitySlab *slab);
	void GetCapabilities(CapabilitySlab *slab, CapabilityTreeNode **capabilityArray, usize count, bool includeMasked);

	CapabilityTreeNode *Insert(CapabilityTreeNode *tree, CapabilityTreeNode *node);
	CapabilityTreeNode *Delete(CapabilityTreeNode *tree, CapabilityTreeNode *node);
	CapabilityTreeNode *Search(CapabilityTreeNode *tree, uptr key);
	CapabilityTreeNode *SearchClose(CapabilityTreeNode *tree, uptr key);

	void Dump(CapabilityTreeNode *tree);
}
