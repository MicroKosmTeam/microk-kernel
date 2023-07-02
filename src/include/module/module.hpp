#pragma once
#include <stdint.h>
#include <proc/process.hpp>
#include <module/buffer.hpp>

namespace MODULE {
	struct BufferNode {
		Buffer *BufferData;

		BufferNode *Next;
	};

	class Module {
	public:
		Module(uint32_t vendorID, uint32_t productID, PROC::Process *proc);

		int RegisterBuffer(uintptr_t virtualBase, BufferType type, size_t size, uint32_t *id);
		Buffer *GetBuffer(uint32_t bufferID);
		void UnregisterBuffer(uint32_t id);

		uint32_t GetVendor() { return VendorID; }
		uint32_t GetProduct() { return ProductID; }
		PROC::Process *GetProcess() { return Process; }

	private:
		BufferNode *AddNode(Buffer *buf);
		void RemoveNode(uint32_t id);
		BufferNode *FindNode(uint32_t id, BufferNode **previous, bool *found);

		uint32_t VendorID;
		uint32_t ProductID;

		PROC::Process *Process;

		BufferNode *BaseNode;
		
		uint32_t MaxBufferID;
		uint32_t GetBufferID() { return ++MaxBufferID; }
	};
}
