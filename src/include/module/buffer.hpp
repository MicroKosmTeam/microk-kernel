#pragma once
#include <stdint.h>
#include <stddef.h>

namespace MODULE {
	enum BufferType {
		BT_INTERRUPT = 0x01,
		BT_MESSAGE = 0x02,
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

	Buffer *CreateBuffer(uint32_t vendorID, uint32_t productID, uint32_t ID, BufferType type, size_t size);
	int LockBuffer(Buffer *buf);
	int UnlockBuffer(Buffer *buf);
	int DeleteBuffer(Buffer *buf);
}
