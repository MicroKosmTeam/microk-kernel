#pragma once
#include <arch/x64/mm/paging.hpp>
#include <stdint.h>

class PageTableManager {
public:
        PageTableManager(PageTable *PML4Address);
        PageTable *PML4;
        void MapMemory(void *virtual_memory, void *physical_memory);
};

extern PageTableManager *GlobalPageTableManager;
