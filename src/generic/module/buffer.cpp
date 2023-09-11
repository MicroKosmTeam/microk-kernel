#include <module/buffer.hpp>
#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <mm/vmm.hpp>
#include <sys/printk.hpp>

namespace MODULE {
BufferManager::BufferManager() {
	BaseNode = new BufferNode;
	BaseNode->Next = NULL;
	BaseNode->BufferData = NULL;

	MaxID = 0;
}

BufferNode *BufferManager::AddNode(Buffer *buf) {
	bool found = false;
	BufferNode *node, *prev;

	node = FindNode(buf->ID, &prev, &found);

	/* Already present, return this one */
	if(found) return node;

	/* If not, prev is now our last node. */
	BufferNode *newNode = new BufferNode;
	newNode->BufferData = buf;
	newNode->Next = NULL;

	prev->Next = newNode;
	
	return prev->Next;
}

void BufferManager::RemoveNode(uint32_t id) {
	bool found = false;
	BufferNode *previous; 
	BufferNode *node = FindNode(id, &previous, &found);

	/* This issue shall be reported */
	if(!found) return;

	previous->Next = node->Next;

	delete node->BufferData;
	delete node;
}

BufferNode *BufferManager::FindNode(uint32_t id, BufferNode **previous, bool *found) {
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

		prev = node;
		if (node->Next == NULL) break;
		node = node->Next;
	}

	*previous = prev;
	*found = false;
	return NULL;
}

Buffer *BufferManager::CreateBuffer(uint32_t vendorID, uint32_t productID, BufferType type, size_t size) {
	Buffer *buf = new Buffer;

	void *pages = PMM::RequestPages(size / PAGE_SIZE + 1);

	buf->ID = GetBufferID();
	buf->Address = (uintptr_t)pages;
	buf->Size = size;
	buf->Type = type;

	buf->OwnerVendorID = vendorID;
	buf->OwnerProductID = productID; 

	buf->Locked = false;
	buf->Pending = false;

	Memset((void*)buf->Address, 0, buf->Size);

	PRINTK::PrintK("Buffer created (ID: %x, VID: %x, PID: %x)\r\n",
			buf->ID, buf->OwnerVendorID, buf->OwnerProductID);

	BufferNode *node = AddNode(buf);
	(void)node;

	return buf;
}

Buffer *BufferManager::GetBuffer(uint32_t id){
	BufferNode *node, *prev;
	bool found = false;

	node = FindNode(id, &prev, &found);

	if(!found) return NULL;

	return node->BufferData;
}
		

int BufferManager::MapBuffer(uint32_t vendorID, uint32_t productID, uint32_t id, VMM::VirtualSpace *vms, uintptr_t address) {
	BufferNode *node, *prev;
	bool found = false;

	node = FindNode(id, &prev, &found);

	if(!found) return -1;

	Buffer *buf = node->BufferData;

	uintptr_t paddr = buf->Address;
	uintptr_t vaddr = address;

	if(vendorID == buf->OwnerVendorID && productID == buf->OwnerProductID) {
		for (size_t i = 0; i < buf->Size; i+=PAGE_SIZE) {
			VMM::MapMemory(vms, (void*)(paddr + i), (void*)(vaddr + i));
		}

		PRINTK::PrintK("Buffer mapped (ID: %x, VID: %x, PID: %x, owner)\r\n",
			buf->ID, buf->OwnerVendorID, buf->OwnerProductID);
	} else {
		for (size_t i = 0; i < buf->Size; i+=PAGE_SIZE) {
			VMM::MapMemory(vms, (void*)(paddr + i), (void*)(vaddr + i), VMM::VMM_PRESENT | VMM::VMM_USER);
		}

		PRINTK::PrintK("Buffer mapped (ID: %x, VID: %x, PID: %x, non-owner)\r\n",
			buf->ID, vendorID, productID);
	}



	return 0;
}

int BufferManager::DeleteBuffer(Buffer *buf) {
	if(!__sync_bool_compare_and_swap(&buf->Pending, false, true)) return -1;

	Memset((void*)buf->Address, 0, buf->Size);
	PMM::FreePages((void*)buf->Address, buf->Size / PAGE_SIZE + 1);

	RemoveNode(buf->ID);

	return 0;
}

int BufferManager::LockBuffer(Buffer *buf) {
	/* Check if operation is pending */
	if(!__sync_bool_compare_and_swap(&buf->Pending, false, true)) return -1;

	if(!__sync_bool_compare_and_swap(&buf->Locked, false, true)) {
		buf->Pending = false;
		return -1;
	}

	buf->Pending = false;

	return 0;
}

int BufferManager::UnlockBuffer(Buffer *buf) {
	/* Check if operation is pending */
	if(!__sync_bool_compare_and_swap(&buf->Pending, false, true)) return -1;

	buf->Locked = false;

	buf->Pending = false;

	return 0;
}

}
