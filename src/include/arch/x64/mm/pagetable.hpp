#pragma once
#include <stdint.h>
#include <arch/x64/mm/paging.hpp>
#include <mm/vmm.hpp>

class PageTableManager : public VMM::VirtualSpace {
public:
        PageTableManager(PageTable *PML4Address);

	void MapMemory(void *physicalMemory, void *virtualMemory, uint64_t flags) override;
	void UnmapMemory(void *virtualMemory) override;
private:
        PageTable *PML4;
};
