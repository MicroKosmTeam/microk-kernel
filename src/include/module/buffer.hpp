#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/vmm.hpp>

namespace MODULE {
	enum BufferType {
		BT_INTERRUPT = 0x01,
		BT_MAILBOX = 0x02,
	};

	struct Buffer {
		/* Unique ID per each module's buffer*/
		uint32_t ID;

		/* The physical address and size 
		 * The module is guaranteed to be contiguous in memory
		 * unless specified
		 */
		uintptr_t Address;
		size_t Size;

		/* Module's type */
		BufferType Type;

		/* The owner is the only one to have RW privileges, the others can  RO */
		uint32_t OwnerVendorID;
		uint32_t OwnerProductID;

		/* The module's being used by a thread, no other can execute instructions */
		bool Locked;

		/* If pending, the module cannot be locked */
		bool Pending;
	};

	struct BufferNode {
		Buffer *BufferData;

		BufferNode *Next;
	};

	class BufferManager {
	public:
		BufferManager();

		Buffer *CreateBuffer(uint32_t vendorID, uint32_t productID, BufferType type, size_t size);
		Buffer *GetBuffer(uint32_t id);
		int MapBuffer(uint32_t vendorID, uint32_t productID, uint32_t id, VMM::VirtualSpace *vms, uintptr_t address);
		int DeleteBuffer(Buffer *buf);

		int LockBuffer(Buffer *buf);
		int UnlockBuffer(Buffer *buf);

	private:
		BufferNode *BaseNode;

		BufferNode *AddNode(Buffer *buf);
		void RemoveNode(uint32_t id);
		BufferNode *FindNode(uint32_t id, BufferNode **previous, bool *found);

		uint32_t MaxID;
		uint32_t GetBufferID() { return ++MaxID; }
	};

}
