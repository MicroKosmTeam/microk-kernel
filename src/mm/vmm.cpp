#include <mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <arch/x64/mm/vmm.hpp>

namespace VMM {
void InitVMM() {
	x86_64::InitVMM();
}

void MapMemory(VirtualSpace *space, void *physicalMemory, void *virtualMemory) {
	space->MapMemory(physicalMemory, virtualMemory, 0);
}
}
