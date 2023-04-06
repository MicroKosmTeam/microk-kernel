#include <mm/vmm.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <arch/x64/mm/vmm.hpp>

namespace VMM {
void InitVMM() {
	x86_64::InitVMM();
}

void MapMemory(void *virtual_memory, void *physical_memory) {
	GlobalPageTableManager->MapMemory((void*)virtual_memory, (void*)physical_memory);
}
}
