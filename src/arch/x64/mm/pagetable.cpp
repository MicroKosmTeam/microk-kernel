#include <mm/pmm.hpp>
#include <mm/memory.hpp>
#include <arch/x64/mm/pageindexer.hpp>
#include <arch/x64/mm/pagetable.hpp>
#include <stdint.h>

#define ENTRIES 512

PageTableManager::PageTableManager(PageTable *PML4Address){
	this->PML4 = PML4Address;
}

#include <sys/printk.hpp>
void PageTableManager::Fork(VMM::VirtualSpace *space, bool higherHalf) {
	PageTable *newPML4 = space->GetTopAddress();

	for (int PDP_i = (higherHalf) ? (256) : (0); PDP_i < ENTRIES; PDP_i++) {
		PageDirectoryEntry PDE;
		PageDirectoryEntry newPDE;

		PDE = PML4->entries[PDP_i];
		newPDE = newPML4->entries[PDP_i];

		PageTable *PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
		PageTable *newPDP;
		if (PDE.GetFlag(PT_Flag::Present)) {
			newPDP = (PageTable*)PMM::RequestPage();
			memset(newPDP, 0, 0x1000);

			newPDE.SetAddress((uint64_t)newPDP >> 12);
			newPDE.SetFlag(PT_Flag::Present, true);
			newPDE.SetFlag(PT_Flag::ReadWrite, true);
			newPDE.SetFlag(PT_Flag::UserSuper, true);
			newPDE.SetFlag(PT_Flag::Global, true);
			newPML4->entries[PDP_i] = newPDE;

			for (int PD_i = 0; PD_i < ENTRIES; PD_i++) {
				PDE = PDP->entries[PD_i];
				newPDE = newPDP->entries[PD_i];

				PageTable *PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
				PageTable *newPD;
				if (PDE.GetFlag(PT_Flag::Present)) {
					newPD = (PageTable*)PMM::RequestPage();
					memset(newPD, 0, 0x1000);
					
					newPDE.SetAddress((uint64_t)newPD >> 12);
					newPDE.SetFlag(PT_Flag::Present, true);
					newPDE.SetFlag(PT_Flag::ReadWrite, true);
					newPDE.SetFlag(PT_Flag::UserSuper, true);
					newPDE.SetFlag(PT_Flag::Global, true);
					newPDP->entries[PD_i] = newPDE;

					for (int PT_i = 0; PT_i < ENTRIES; PT_i++) {
						PDE = PD->entries[PT_i];
						newPDE = newPD->entries[PT_i];

						PageTable *PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
						PageTable *newPT;
						if (PDE.GetFlag(PT_Flag::Present)) {
							newPT = (PageTable*)PMM::RequestPage();
							memset(newPT, 0, 0x1000);
							newPDE.SetAddress((uint64_t)newPT >> 12);
							newPDE.SetFlag(PT_Flag::Present, true);
							newPDE.SetFlag(PT_Flag::ReadWrite, true);
							newPDE.SetFlag(PT_Flag::UserSuper, true);
							newPDE.SetFlag(PT_Flag::Global, true);
							newPD->entries[PT_i] = newPDE;

							for (int P_i = 0; P_i < ENTRIES; P_i++) {
								PDE = PT->entries[P_i];
								newPDE = newPT->entries[P_i];
								
								if (PDE.GetFlag(PT_Flag::Present)) {
									newPDE.SetAddress(PDE.GetAddress());
									newPDE.SetFlag(PT_Flag::Present, true);

									newPDE.SetFlag(PT_Flag::ReadWrite, PDE.GetFlag(PT_Flag::ReadWrite));
									newPDE.SetFlag(PT_Flag::UserSuper, PDE.GetFlag(PT_Flag::UserSuper));
									newPDE.SetFlag(PT_Flag::Global, PDE.GetFlag(PT_Flag::Global));

									newPT->entries[P_i] = newPDE;
								}
							}
							

						}

					}
				}
							
			}
		}
	}
}

void PageTableManager::MapMemory(void *physicalMemory, void *virtualMemory, uint64_t flags){
	PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
	PageDirectoryEntry PDE;

	PDE = PML4->entries[indexer.PDP_i];
	PageTable* PDP;
	if (!PDE.GetFlag(PT_Flag::Present)) {
		PDP = (PageTable*)PMM::RequestPage();
		memset(PDP, 0, 0x1000);
		PDE.SetAddress((uint64_t)PDP >> 12);
		PDE.SetFlag(PT_Flag::Present, true);
		PDE.SetFlag(PT_Flag::ReadWrite, true);
		PDE.SetFlag(PT_Flag::UserSuper, true);
		PDE.SetFlag(PT_Flag::Global, true);
		PML4->entries[indexer.PDP_i] = PDE;
	} else {
		PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	}

	PDE = PDP->entries[indexer.PD_i];
	PageTable* PD;
	if (!PDE.GetFlag(PT_Flag::Present)) {
		PD = (PageTable*)PMM::RequestPage();
		memset(PD, 0, 0x1000);
		PDE.SetAddress((uint64_t)PD >> 12);
		PDE.SetFlag(PT_Flag::Present, true);
		PDE.SetFlag(PT_Flag::ReadWrite, true);
		PDE.SetFlag(PT_Flag::UserSuper, true);
		PDE.SetFlag(PT_Flag::Global, true);
		PDP->entries[indexer.PD_i] = PDE;
	} else {
		PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	}

	PDE = PD->entries[indexer.PT_i];
	PageTable* PT;
	if (!PDE.GetFlag(PT_Flag::Present)) {
		PT = (PageTable*)PMM::RequestPage();
		memset(PT, 0, 0x1000);
		PDE.SetAddress((uint64_t)PT >> 12);
		PDE.SetFlag(PT_Flag::Present, true);
		PDE.SetFlag(PT_Flag::ReadWrite, true);
		PDE.SetFlag(PT_Flag::UserSuper, true);
		PDE.SetFlag(PT_Flag::Global, true);
		PD->entries[indexer.PT_i] = PDE;
	} else {
		PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	}

	PDE = PT->entries[indexer.P_i];
	PDE.SetAddress((uint64_t)physicalMemory >> 12);
	PDE.SetFlag(PT_Flag::Present, true);
	PDE.SetFlag(PT_Flag::ReadWrite, true);
	PDE.SetFlag(PT_Flag::UserSuper, true);
	PDE.SetFlag(PT_Flag::Global, true);

	PT->entries[indexer.P_i] = PDE;

	asm volatile("invlpg (%0)" ::"r" (virtualMemory) : "memory");
}
	
void PageTableManager::UnmapMemory(void *virtualMemory) {
	PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
	PageDirectoryEntry PDE;

	PDE = PML4->entries[indexer.PDP_i];
	PageTable* PDP;
	if (PDE.GetFlag(PT_Flag::Present)) {
		PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	} else return;

	PDE = PDP->entries[indexer.PD_i];
	PageTable* PD;
	if (PDE.GetFlag(PT_Flag::Present)) {
		PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	} else return;

	PDE = PD->entries[indexer.PT_i];
	PageTable* PT;
	if (PDE.GetFlag(PT_Flag::Present)) {
		PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	} else return;

	PDE = PT->entries[indexer.P_i];
	if (PDE.GetFlag(PT_Flag::Present)) PDE.SetFlag(PT_Flag::Present, false);
	else return;
	PT->entries[indexer.P_i] = PDE;
	
	asm volatile("invlpg (%0)" ::"r" (virtualMemory) : "memory");
}

void *PageTableManager::GetPhysicalAddress(void *virtualMemory) {
	PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
	PageDirectoryEntry PDE;
	void *address;

	PDE = PML4->entries[indexer.PDP_i];
	PageTable* PDP;
	if (PDE.GetFlag(PT_Flag::Present)) {
		PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	} else return NULL;

	PDE = PDP->entries[indexer.PD_i];
	PageTable* PD;
	if (PDE.GetFlag(PT_Flag::Present)) {
		PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	} else return NULL;

	PDE = PD->entries[indexer.PT_i];
	PageTable* PT;
	if (PDE.GetFlag(PT_Flag::Present)) {
		PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	} else return NULL;

	PDE = PT->entries[indexer.P_i];
	if (PDE.GetFlag(PT_Flag::Present)) {
		address = (void*)((uint64_t)PDE.GetAddress() << 12);
	} else return NULL;

	return address;
}
