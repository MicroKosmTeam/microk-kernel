#pragma once
#include <stdint.h>

namespace VMM {
	enum VirtualMemoryFlags {
		VMM_READWRITE = 0b0001,
		VMM_USERSUPER = 0b0010,
		VMM_NOEXECUTE = 0b0100,
		VMM_LARGEPAGE = 0b1000
	};

	class VirtualSpace {
	public:
		virtual void MapMemory(void *physicalMemory, void *virtualMemory, uint64_t flags) = 0;
		virtual void UnmapMemory(void *virtualMemory) = 0;
		virtual void *GetTopAddress() = 0;
	private:
	};

	void InitVMM();
	VirtualSpace *NewVirtualSpace();
	VirtualSpace *Fork(VirtualSpace *space);
	void LoadVirtualSpace(VMM::VirtualSpace *space);
	void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory);
}
