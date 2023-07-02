#include <module/buffer.hpp>
#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <mm/vmm.hpp>
#include <sys/printk.hpp>

namespace MODULE {
Buffer *CreateBuffer(uint32_t vendorID, uint32_t productID, uint32_t ID, BufferType type, size_t size) {
	Buffer *buf = new Buffer;

	buf->ID = ID;
	buf->Address = PMM::RequestPages(size / PAGE_SIZE + 1);
	buf->Size = size;
	buf->Type = type;

	buf->OwnerVendorID = vendorID;
	buf->OwnerProductID = productID; 

	buf->Locked = false;
	buf->Pending = false;

	memset(buf->Address, 0, buf->Size);

	return buf;
}

int LockBuffer(Buffer *buf) {
	/* Check if operation is pending */
	if(!__sync_bool_compare_and_swap(&buf->Pending, false, true)) return -1;

	if(!__sync_bool_compare_and_swap(&buf->Locked, false, true)) {
		buf->Pending = false;
		return -1;
	}

	buf->Pending = false;
	return 0;
}

int UnlockBuffer(Buffer *buf) {
	/* Check if operation is pending */
	if(!__sync_bool_compare_and_swap(&buf->Pending, false, true)) return -1;

	buf->Locked = false;

	buf->Pending = false;

	return 0;
}

int DeleteBuffer(Buffer *buf) {
	if(!__sync_bool_compare_and_swap(&buf->Pending, false, true)) return -1;

	memset(buf->Address, 0, buf->Size);
	PMM::FreePages(buf->Address, buf->Size / PAGE_SIZE + 1);

	return 0;
}

}
