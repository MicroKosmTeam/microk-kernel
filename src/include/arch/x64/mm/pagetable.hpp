#pragma once
#include <cstdint.hpp>
#include <arch/x64/mm/paging.hpp>
#include <mm/vmm.hpp>

class PageTableManager : public VMM::VirtualSpace {
public:
        PageTableManager(PageTable *PML4Address);

	void *GetPhysicalAddress(void *virtualMemory) override;
	void Fork(VMM::VirtualSpace *space, bool higherHalf) override;
	void MapMemory(void *physicalMemory, void *virtualMemory, u64 flags) override;
	void UnmapMemory(void *virtualMemory) override;

	void *GetTopAddress() override {
		return PML4;
	}
private:
        PageTable *PML4;
};
