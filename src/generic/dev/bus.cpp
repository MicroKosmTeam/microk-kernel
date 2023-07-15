#include <dev/bus.hpp>
#include <sys/printk.hpp>
#include <mm/string.hpp>
#include <mm/memory.hpp>

namespace DEV {
BusManager::BusManager() {
	BaseNode = new BusNode;

	BaseNode->VendorID = 0;
	BaseNode->ProductID = 0;

	BaseNode->Next = NULL;
}

BusNode *BusManager::AddNode(const char *busName, uint32_t vendorID, uint32_t productID) {
	bool found = false;
	BusNode *node, *prev;

	node = FindNode(busName, vendorID, productID, &prev, &found);

	/* Already present, return this one */
	if(found) return node;

	/* If not, prev is now our last node. */
	BusNode *newNode = new BusNode;
	newNode->VendorID = 0;
	newNode->ProductID = 0;
	newNode->Next = NULL;

	prev->Next = newNode;
	
	return prev->Next;
}

void BusManager::RemoveNode(const char *busName, uint32_t vendorID, uint32_t productID) {
	bool found = false;
	BusNode *previous; 
	BusNode *node = FindNode(busName, vendorID, productID, &previous, &found);

	/* This issue shall be reported */
	if(!found) return;

	previous->Next = node->Next;

	delete node;
}

BusNode *BusManager::FindNode(const char *busName, uint32_t vendorID, uint32_t productID, BusNode **previous, bool *found) {
	BusNode *node = BaseNode->Next, *prev = BaseNode;

	if (node == NULL) {
		*previous = prev;
		*found = false;
		return NULL;
	}

	size_t stringComparison = strcmp(node->BusName, busName);
	bool areVendorProductGeneric = (vendorID == 0 && productID == 0);

	while(true) {
		if (stringComparison == 0) {
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

		if (node->Next == NULL) break;
		prev = node;
		node = node->Next;
	}

	*previous = prev;
	*found = false;
	return NULL;
}

int BusManager::RegisterBusDriver(const char *busName, uint32_t vendorID, uint32_t productID) {
	BusNode *node = AddNode(busName, vendorID, productID);

	if(node->VendorID != 0 || node->ProductID != 0) return -1;

	strcpy(node->BusName, busName);
	node->VendorID = vendorID;
	node->ProductID = productID;

	PRINTK::PrintK("Registered bus (BUS: %s, VID: %x, PID: %x)\r\n",
			node->BusName,
			node->VendorID,
			node->ProductID);

	return 0;
}

void BusManager::GetBusDriver(const char *busName, uint32_t *vendorID, uint32_t *productID) {
	BusNode *node, *prev;
	bool found;
	node = FindNode(busName, 0, 0, &prev, &found);

	if (!found) {
		*vendorID = 0;
		*productID = 0;

		return;
	}

	*vendorID = node->VendorID;
	*productID = node->ProductID;
}

void BusManager::UnregisterBusDriver(const char *busName, uint32_t vendorID, uint32_t productID) {
	RemoveNode(busName, vendorID, productID);
}

}
