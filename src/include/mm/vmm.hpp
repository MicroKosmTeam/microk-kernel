#pragma once
#include <init/kinfo.hpp>

namespace VMM {
	enum VirtualMemoryFlags {
		VMM_READWRITE = 0b0001,
		VMM_USERSUPER = 0b0010,
		VMM_NOEXECUTE = 0b0100,
		VMM_LARGEPAGE = 0b1000
	};

	class KernelVirtualSpace {
	public:
		virtual void MapMemory(void *physicalMemory, void *virtualMemory, uint64_t flags) = 0;
		virtual void UnmapMemory(void *virtualMemory) = 0;
	private:
		virtual int Init();
	};

	class ProcessVirtualSpace {
	public:
		virtual void MapMemory(void *physicalMemory, void *virtualMemory, uint64_t flags) = 0;
		virtual void UnmapMemory(void *virtualMemory) = 0;
	private:
		virtual int Init();
	};

	void InitVMM();
	void MapMemory(void *virtual_memory, void *physical_memory);
}
