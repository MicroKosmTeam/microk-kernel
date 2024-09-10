#pragma once
#include <object.hpp>

namespace SLAB {
	CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node);
	CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node);
	CapabilityNode *FindSlabNode(CapabilitySlab *slab, uptr address);
	CapabilityTreeNode *FindElementInNode(CapabilityNode *node, uptr address);
	CapabilityTreeNode *FindElementInSlab(CapabilitySlab *slab, uptr address);
	
	CapabilityTreeNode *AllocateFreeSlotInSlab(CapabilitySlab *slab);
	CapabilityTreeNode *ClaimSlotInSlab(CapabilitySlab *slab);
	CapabilityTreeNode *Insert(CapabilityTreeNode *tree, CapabilityTreeNode *node);
	CapabilityTreeNode *Delete(CapabilityTreeNode *tree, CapabilityTreeNode *node);
	CapabilityTreeNode *Search(CapabilityTreeNode *tree, uptr key);

	void Dump(CapabilityTreeNode *tree);
}
