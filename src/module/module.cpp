#include <module/module.hpp>
#include <sys/printk.hpp>

namespace MODULE {
Module::Module(uint32_t vendorID, uint32_t productID, PROC::Process *proc) {
	VendorID = vendorID;
	ProductID = productID;
	Process = proc;

	MaxBufferID = 0;
	
	BaseNode = new BufferNode;
	BaseNode->BufferData = NULL;
	BaseNode->Next = NULL;
}

BufferNode *Module::AddNode(Buffer *buf) {
	bool found = false;
	BufferNode *node, *prev;

	node = FindNode(buf->ID, &prev, &found);

	/* Already present, return this one */
	if(found) return node;

	/* If not, prev is now our last node. */
	BufferNode *newNode = new BufferNode;
	newNode->BufferData = NULL;
	newNode->Next = NULL;

	prev->Next = newNode;
	
	return prev->Next;
}

void Module::RemoveNode(uint32_t id) {
	bool found = false;
	BufferNode *previous; 
	BufferNode *node = FindNode(id, &previous, &found);

	/* This issue shall be reported */
	if(!found) return;

	previous->Next = node->Next;

	DeleteBuffer(node->BufferData);
	delete node->BufferData;
	delete node;
}

BufferNode *Module::FindNode(uint32_t id, BufferNode **previous, bool *found) {
	BufferNode *node = BaseNode->Next, *prev = BaseNode;

	if (node == NULL) {
		*previous = prev;
		*found = false;
		return NULL;
	}


	while(true) {
		if (node->BufferData->ID == id) {
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

		
int Module::RegisterBuffer(uintptr_t virtualBase, BufferType type, size_t size, uint32_t *id) {
	Buffer *buf = CreateBuffer(GetVendor(), GetProduct(), GetBufferID(), type, size);

	BufferNode *node = AddNode(buf);

	node->BufferData = buf;
	
	VMM::VirtualSpace *vms = Process->GetVirtualMemorySpace();

	for (int i = 0; i < size / PAGE_SIZE + 1; i += PAGE_SIZE) {
		VMM::MapMemory(vms, buf->Address + i, virtualBase + i);
	}

	*id = node->BufferData->ID;

	PRINTK::PrintK("Registered buffer (ID: %x, TYPE: %x ADDR: 0x%x, SIZE: %d)\r\n",
			node->BufferData->ID,
			node->BufferData->Type,
			node->BufferData->Address,
			node->BufferData->Size);

	return 0;
}

Buffer *Module::GetBuffer(uint32_t bufferID) {
	BufferNode *node, *prev;
	bool found;
	node = FindNode(bufferID, &prev, &found);

	if (!found) return NULL;
	return node->BufferData;
}

void Module::UnregisterBuffer(uint32_t id) {
	RemoveNode(id);

	PRINTK::PrintK("Unregistered buffer (ID: %x)\r\n", id);
}
}
