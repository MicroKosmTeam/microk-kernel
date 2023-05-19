#pragma once
#include <stdint.h>

namespace VMM {
	enum VirtualMemoryFlags {
		VMM_READWRITE  = 0b00000001,
		VMM_NOEXECUTE  = 0b00000010,
		VMM_LARGEPAGE  = 0b00000100,
		VMM_LARGERPAGE = 0b00001000,
	};

	class VirtualSpace {
	public:
		virtual void Fork(VirtualSpace *space) = 0;
		virtual void MapMemory(void *physicalMemory, void *virtualMemory, uint64_t flags) = 0;
		virtual void UnmapMemory(void *virtualMemory) = 0;
		virtual void *GetTopAddress() = 0;
	private:
	};

	void InitVMM();
	VirtualSpace *NewVirtualSpace();
	VirtualSpace *NewKernelVirtualSpace();
	VirtualSpace *NewModuleVirtualSpace();
	VirtualSpace *Fork(VirtualSpace *space);
	void LoadVirtualSpace(VMM::VirtualSpace *space);
	void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory);
}
