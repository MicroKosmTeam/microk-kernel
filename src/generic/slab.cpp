#include <slab.hpp>
#include <vmm.hpp>
#include <list.hpp>

namespace SLAB {

CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node) {
	AddElementToList(node, &slab->FreeSlabs);

	for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
		node->Slots[i].IsClaimed = 0;
		node->Slots[i].Type = NULL_CAPABILITY;
	}

	return node;
}

CapabilityNode *FindSlabNode(CapabilitySlab *slab, uptr address) {
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	ROUND_DOWN_TO_PAGE(address);

	while(node) {
		if (address == (uptr)node) {
			return node;
		} else {
			node = (CapabilityNode*)node->Next;
		}
	}

	node = (CapabilityNode*)slab->FullSlabs.Head;

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
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	if(node == NULL) {
		node = (CapabilityNode*)slab->FreeSlabs.Head;
		if (node == NULL) {
			return NULL;
		}
	}

	while(node) {
		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			Capability *cap = &node->Slots[i];
			if(cap->Type == NULL_CAPABILITY) {
				cap->Type = RESERVED_SLOT;

				if (node->FreeElements == CAPABILITIES_PER_NODE) {
					RemoveElementFromList(node, &slab->FreeSlabs);
					AddElementToList(node, &slab->UsedSlabs);
					--node->FreeElements;
				} else if (node->FreeElements == 1) {
					RemoveElementFromList(node, &slab->UsedSlabs);
					AddElementToList(node, &slab->FullSlabs);
					node->FreeElements = 0;
				}
				
				return cap;
			}
			
		}
	}

	return NULL;
}

Capability *ClaimSlotInSlab(CapabilitySlab *slab) {
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	bool done = false;
	do {
		while(node) {
			for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
				Capability *cap = &node->Slots[i];
				if(cap->Type != NULL_CAPABILITY && !cap->IsClaimed) {
					cap->IsClaimed = true;

					return cap;
				}			
			}
		}
		
		node = (CapabilityNode*)slab->FullSlabs.Head;
		done = true;
	} while(!done);

	return NULL;

}
}
