#include <module/section.hpp>
#include <sys/printk.hpp>
#include <mm/string.hpp>
#include <mm/memory.hpp>

namespace MODULE {
SectionManager::SectionManager() {
	BaseNode = new SectionNode;

	BaseNode->VendorID = 0;
	BaseNode->ProductID = 0;

	BaseNode->Next = NULL;
}

SectionNode *SectionManager::AddNode(const char *sectionName, uint32_t vendorID, uint32_t productID) {
	bool found = false;
	SectionNode *node, *prev;

	node = FindNode(sectionName, vendorID, productID, &prev, &found);

	/* Already present, return this one */
	if(found) return node;

	/* If not, prev is now our last node. */
	SectionNode *newNode = new SectionNode;
	newNode->VendorID = vendorID;
	newNode->ProductID = productID;
	newNode->Next = NULL;

	prev->Next = newNode;
	
	return prev->Next;
}

void SectionManager::RemoveNode(const char *sectionName, uint32_t vendorID, uint32_t productID) {
	bool found = false;
	SectionNode *previous; 
	SectionNode *node = FindNode(sectionName, vendorID, productID, &previous, &found);

	/* This issue shall be reported */
	if(!found) return;

	previous->Next = node->Next;

	delete node;
}

SectionNode *SectionManager::FindNode(const char *sectionName, uint32_t vendorID, uint32_t productID, SectionNode **previous, bool *found) {
	SectionNode *node = BaseNode->Next, *prev = BaseNode;

	if (node == NULL) {
		*previous = prev;
		*found = false;
		return NULL;
	}

	bool areVendorProductGeneric = (vendorID == 0 && productID == 0) ? true : false;

	while(true) {
		if (strcmp(node->SectionName, sectionName) == 0) {
			if (areVendorProductGeneric || (node->VendorID == vendorID && node->ProductID == productID)) {
				*found = true;
				*previous = prev;
				return node;
			} else {
				*found = false;
				*previous = prev;
				return NULL;
			}
		}

		prev = node;
		if (node->Next == NULL) break;
		node = node->Next;
	}

	*previous = prev;
	*found = false;
	return NULL;
}

int SectionManager::RegisterSectionDriver(const char *sectionName, uint32_t vendorID, uint32_t productID) {
	SectionNode *node = AddNode(sectionName, vendorID, productID);

	if(node->VendorID != 0 || node->ProductID != 0) return -1;

	strcpy(node->SectionName, sectionName);
	node->VendorID = vendorID;
	node->ProductID = productID;

	PRINTK::PrintK("Registered section (SECTION: %s, VID: %x, PID: %x)\r\n",
			node->SectionName,
			node->VendorID,
			node->ProductID);

	return 0;
}

void SectionManager::GetSectionDriver(const char *sectionName, uint32_t *vendorID, uint32_t *productID) {
	SectionNode *node, *prev;
	bool found;
	node = FindNode(sectionName, 0, 0, &prev, &found);

	if (!found) {
		*vendorID = 0;
		*productID = 0;

		return;
	}

	*vendorID = node->VendorID;
	*productID = node->ProductID;
}

void SectionManager::UnregisterSectionDriver(const char *sectionName, uint32_t vendorID, uint32_t productID) {
	RemoveNode(sectionName, vendorID, productID);
}

}
