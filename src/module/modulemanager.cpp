#include <module/modulemanager.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>

namespace MODULE {
Manager::Manager() {
	BaseNode = new ModuleNode;
	BaseNode->Next = NULL;
	BaseNode->ModuleData = NULL;
}

ModuleNode *Manager::AddNode(uint32_t vendorID, uint32_t productID) {
	bool found = false;
	ModuleNode *node, *prev;

	node = FindNode(vendorID, productID, &prev, &found);

	/* Already present, return this one */
	if(found) return node;

	/* If not, prev is now our last node. */
	ModuleNode *newNode = new ModuleNode;
	newNode->ModuleData = NULL;
	newNode->Next = NULL;

	prev->Next = newNode;
	
	return prev->Next;
}

void Manager::RemoveNode(uint32_t vendorID, uint32_t productID) {
	bool found = false;
	ModuleNode *previous; 
	ModuleNode *node = FindNode(vendorID, productID, &previous, &found);

	/* This issue shall be reported */
	if(!found) return;

	previous->Next = node->Next;

	delete node->ModuleData;
	delete node;
}

ModuleNode *Manager::FindNode(uint32_t vendorID, uint32_t productID, ModuleNode **previous, bool *found) {
	ModuleNode *node = BaseNode->Next, *prev = BaseNode;

	if (node == NULL) {
		*previous = prev;
		*found = false;
		return NULL;
	}

	while(true) {
		if (node->ModuleData->GetVendor() == vendorID &&
		    node->ModuleData->GetProduct() == productID) {
			*found = true;
			*previous = prev;
			return node;
		}

		if (node->Next == NULL) break;
		prev = node;
		node = node->Next;
	}

	*previous = prev;
	*found = false;
	return NULL;
}

int Manager::RegisterModule(PROC::Process *process, uint32_t vendorID, uint32_t productID) {
	ModuleNode *node = AddNode(vendorID, productID);

	if(node->ModuleData != NULL) return -1;

	node->ModuleData = new Module(vendorID, productID, process);

	PRINTK::PrintK("Registered module (VID: %x, PID: %x)\r\n",
			node->ModuleData->GetVendor(),
			node->ModuleData->GetProduct());

	return 0;
}

Module *Manager::GetModule(uint32_t vendorID, uint32_t productID) {
	ModuleNode *node, *prev;
	bool found;
	node = FindNode(vendorID, productID, &prev, &found);

	if (!found) return NULL;
	return node->ModuleData;
}

Module *Manager::GetModule(size_t PID) {
	ModuleNode *node = BaseNode->Next;

	if (node == NULL) {
		return NULL;
	}

	while(true) {
		if (node->ModuleData->GetProcess()->GetPID() == PID) {
			return node->ModuleData;
		}

		if (node->Next == NULL) break;
		node = node->Next;
	}
}

void Manager::UnregisterModule(uint32_t vendorID, uint32_t productID) {
	RemoveNode(vendorID, productID);
}
}
