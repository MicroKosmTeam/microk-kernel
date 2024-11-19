#include <slab.hpp>
#include <vmm.hpp>
#include <list.hpp>
#include <math.hpp>
#include <panic.hpp>
#include <printk.hpp>

namespace SLAB {
CapabilityNode *AddSlabNode(CapabilitySlab *slab, CapabilityNode *node) {
	node->Next = node->Previous = NULL;
	AddElementToList(node, &slab->FreeSlabs);
		
	node->FreeElements = CAPABILITIES_PER_NODE;

	for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
		node->Slots[i].IsMasked = 0;
		node->Slots[i].Type = NULL_CAPABILITY;
	}

	return node;
}

CapabilityTreeNode *AllocateSlabSlot(CapabilitySlab *slab) {
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	if(node == NULL) {
		node = (CapabilityNode*)slab->FreeSlabs.Head;
		if (node == NULL) {
			return NULL;
		}
	}

	while(node) {
		for (usize i = 0; i < CAPABILITIES_PER_NODE; ++i) {
			CapabilityTreeNode *cap = &node->Slots[i];
			if(cap->Type == NULL_CAPABILITY) {
				cap->Type = RESERVED_SLOT;

				if (node->FreeElements == CAPABILITIES_PER_NODE) {
					RemoveElementFromList(node, &slab->FreeSlabs);
					AddElementToList(node, &slab->UsedSlabs);
				} else if (node->FreeElements == 1) {
					RemoveElementFromList(node, &slab->UsedSlabs);
					AddElementToList(node, &slab->FullSlabs);
				}
					
				--node->FreeElements;
				
				return cap;
			}
			
		}
	}

	return NULL;
}

void FreeSlabSlot(CapabilitySlab *slab, CapabilityTreeNode *capability) {
	capability->Type = NULL_CAPABILITY;

	uptr nodeAddress = (uptr)capability;
	ROUND_DOWN_TO_PAGE(nodeAddress);

	CapabilityNode *node = (CapabilityNode*)nodeAddress;

	++node->FreeElements;

	if (node->FreeElements == CAPABILITIES_PER_NODE) {
		RemoveElementFromList(node, &slab->UsedSlabs);
		AddElementToList(node, &slab->FreeSlabs);
	} else if (node->FreeElements == 1) {
		RemoveElementFromList(node, &slab->FullSlabs);
		AddElementToList(node, &slab->UsedSlabs);
	}

	return;
}

usize GetFreeSlabSlots(CapabilitySlab *slab) {
	CapabilityNode *node = (CapabilityNode*)slab->UsedSlabs.Head;

	usize slots = 0;

	while(node) {
		slots += node->FreeElements;
		node = (CapabilityNode*)node->Next;
	}

	node = (CapabilityNode*)slab->FreeSlabs.Head;
	
	while(node) {
		slots += node->FreeElements;
		node = (CapabilityNode*)node->Next;
	}

	return slots;
}

CapabilityTreeNode *Skew(CapabilityTreeNode *tree) {
	if (tree == NULL) {
		return NULL;
	} else if (tree->Left == NULL) {
		return tree;
	} else if (tree->Left->Level == tree->Level) {
		CapabilityTreeNode *left = tree->Left;
		tree->Left = left->Right;
		left->Right = tree;
		return left;
	} else {
		return tree;
	}
}

CapabilityTreeNode *Split(CapabilityTreeNode *tree) {
	if (tree == NULL) {
		return NULL;
	} else if (tree->Right == NULL || tree->Right->Right == NULL) {
		return tree;
	} else if (tree->Level == tree->Right->Right->Level) {
		CapabilityTreeNode *right = tree->Right;
		tree->Right = right->Left;
		right->Left = tree;
		++right->Level;
		return right;
	} else {
		return tree;
	}
}

CapabilityTreeNode *Insert(CapabilityTreeNode *tree, CapabilityTreeNode *node) {
	if (tree == NULL) {
		node->Level = 1;
		node->Left = node->Right = NULL;
		return node;
	} else if (node->Object < tree->Object) {
		tree->Left = Insert(tree->Left, node);
	} else if (node->Object > tree->Object) {
		tree->Right = Insert(tree->Right, node);
	} else /* (node->Object == tree->Object) */ {
		PANIC("SAME KEY DOUBLE INSERTED");
	}
	
	tree = Skew(tree);
	tree = Split(tree);

	return tree;
}

CapabilityTreeNode *DecreaseLevel(CapabilityTreeNode *tree) {
	u32 shouldBe = MATH::Min(tree->Left->Level, tree->Right->Level) + 1;

	if (tree->Left && tree->Right) {
		if (shouldBe < tree->Level) {
			tree->Level = shouldBe;
			if (shouldBe < tree->Right->Level) {
				tree->Right->Level = shouldBe;
			}
		}
	}

	return tree;
}

CapabilityTreeNode *Predecessor(CapabilityTreeNode *curNode) {
	curNode = curNode->Left;

	while (curNode->Right != NULL) {
        	curNode = curNode->Right;
	}

	return curNode;
}

CapabilityTreeNode *Successor(CapabilityTreeNode *curNode) {
	curNode = curNode->Right;

	while (curNode->Left != NULL) {
		curNode = curNode->Left;
	}

	return curNode;
}

CapabilityTreeNode *Delete(CapabilityTreeNode *tree, CapabilityTreeNode *node) {
	if (tree == NULL) {
		return tree;
	} else if (node->Object < tree->Object) {
		tree->Left = Delete(tree->Left, node);
	} else if (node->Object > tree->Object) {
		tree->Right = Delete(tree->Right, node);
	} else {
		if (tree->Left == NULL && tree->Right == NULL) {
			// TODO: Free slot */
			return NULL;
		} else if (tree->Left == NULL) {
			CapabilityTreeNode *successor = Successor(tree);

			/* TODO:ZEROCOPY CAPABILITY */
			Capability *treeCapability = (Capability*)tree;
			Capability *successorCapability = (Capability*)successor;
			*treeCapability = *successorCapability;
			
			tree->Right = Delete(tree->Right, successor);
		} else {
			CapabilityTreeNode *predecessor = Successor(tree);

			/* TODO:ZEROCOPY CAPABILITY */
			Capability *treeCapability = (Capability*)tree;
			Capability *predecessorCapability = (Capability*)predecessor;
			*treeCapability = *predecessorCapability;
			
			tree->Left = Delete(tree->Left, predecessor);
		}
	}

	tree = DecreaseLevel(tree);
	tree = Skew(tree);
	tree->Right = Skew(tree->Right);

	if (tree->Right != NULL && tree->Right->Right != NULL) {
		tree->Right->Right = Skew(tree->Right->Right);
	}

	tree = Split(tree);
	tree->Right = Split(tree);

	return tree;
}
	
CapabilityTreeNode *Search(CapabilityTreeNode *tree, uptr key) {
	CapabilityTreeNode *node = tree;

	while (node != NULL) {
		if (key == node->Object) {
			return node;
		} else if (key > node->Object) {
			node = node->Right;
		} else {
			node = node->Left;
		}
	}

	return NULL;
}

CapabilityTreeNode *SearchClose(CapabilityTreeNode *tree, uptr key) {
	CapabilityTreeNode *node = tree;

	while (node != NULL) {
		if ((node->Right == NULL && node->Left == NULL) || key == node->Object) {
			return node;
		} else if (key > node->Object) {
			node = node->Right;
		} else {
			node = node->Left;
		}
	}

	return NULL;
}



void Dump(CapabilityTreeNode *tree) {
	if (tree == NULL) {
		return;
	}

	PRINTK::PrintK(PRINTK_DEBUG "#%d 0x%x: 0x%x\r\n"
		       "     -> L 0x%x: 0x%x\r\n"
		       "     -> R 0x%x: 0x%x\r\n",
		       tree->Level,
		       tree, tree->Object,
		       tree->Left, tree->Left ? tree->Left->Object : 0,
		       tree->Right, tree->Right ? tree->Right->Object : 0);

	Dump(tree->Left);
	Dump(tree->Right);
}
}
