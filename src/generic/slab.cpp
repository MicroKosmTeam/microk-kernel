#include <slab.hpp>
#include <vmm.hpp>

namespace SLAB {

CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node) {
	CapabilityNode *tailNode = (CapabilityNode*)slab->Nodes.Tail;

	tailNode->Next = node;
	node->Next = NULL;
	node->Previous = tailNode;
	slab->Nodes.Tail = node;

	slab->NodesAvailable = true;

	for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
		node->Slots[i].Type = NULL_CAPABILITY;
	}

	return node;
}

CapabilityNode *FindSlabNode(CapabilitySlab *slab, uptr address) {
	CapabilityNode *node = (CapabilityNode*)slab->Nodes.Head;

	ROUND_DOWN_TO_PAGE(address);

	while(node) {
		if (address == (uptr)node) {
			return node;
		} else {
			node = (CapabilityNode*)node->Next;
		}
	}

	return NULL;
}

Capability *FindElementInNode(CapabilityNode *node, uptr address) {
	if (address >= (uptr)node + PAGE_SIZE) {
		return NULL;
	}

	uptr elementStart = (uptr)node - sizeof(ListHead);
	uptr offset = address - elementStart;

	if (offset % sizeof(Capability) == 0) {
		return (Capability*)address;
	}

	return NULL;
}


Capability *FindElementInSlab(CapabilitySlab *slab, uptr address) {
	CapabilityNode *node = FindSlabNode(slab, address);
	if (node == NULL) {
		return NULL;
	}

	Capability *cap = FindElementInNode(node, address);
	return cap;
}
	
Capability *AllocateFreeSlotInSlab(CapabilitySlab *slab) {
	CapabilityNode *node = (CapabilityNode*)slab->Nodes.Head;

	if(!slab->NodesAvailable) {
		return NULL;
	}

	while(node) {
		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			Capability *cap = &node->Slots[i];
			if(cap->Type == NULL_CAPABILITY) {
				cap->Type = RESERVED_SLOT;
				return cap;
			}
			
		}
	}

	slab->NodesAvailable = false;
	return NULL;
}

}
