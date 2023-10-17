#pragma once
#include <cstdint.hpp>


#define PAGE_SIZE 0x1000

namespace VMM {
	enum VirtualMemoryFlags {
		VMM_PRESENT    = 0b00000001,
		VMM_READWRITE  = 0b00000010,
		VMM_USER       = 0b00000100,
		VMM_NOEXECUTE  = 0b00001000,
		VMM_GLOBAL     = 0b00010000,
	};

	class VirtualSpace {
	public:
		virtual ~VirtualSpace() { };
		virtual void *GetPhysicalAddress(void *virtualMemory) = 0;
		virtual void Fork(VirtualSpace *space, bool higherHalf) = 0;
		virtual void MapMemory(void *physicalMemory, void *virtualMemory, u64 flags) = 0;
		virtual void UnmapMemory(void *virtualMemory) = 0;
		virtual void *GetTopAddress() = 0;
	private:
	};
	
	struct COWMetadata {
		uptr PhysicalAddressOfOriginal;
		uptr PhysicalAddressOfCopy;

		usize VirtualReferences;
		uptr VirtualAddresses[];
	};

	struct PageMetadata {
		bool IsCOW;
		union {
			uptr PhysicalAddress;
			COWMetadata *COW;
		} Data;
	};

	struct PageList {
		usize PageCount;
		usize AllocatedSize;
		PageMetadata Pages[];
	};

	void InitVMM();
	VirtualSpace *AllocateVirtualSpace();
	VirtualSpace *NewVirtualSpace();
	void LoadVirtualSpace(VMM::VirtualSpace *space);
	void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory);
	void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory, u64 flags);
}
