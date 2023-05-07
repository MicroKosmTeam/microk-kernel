#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mkmi.h>

uint64_t *KRNLSYMTABLE;
uint64_t PHYSBASE;

void SetupSymtable() {
	KInfo *info = GetInfo();

//	for (int i = 0; i < CONFIG_SYMBOL_TABLE_PAGES * 0x1000; i += 0x1000) {
		PHYSBASE = PMM::RequestPage();
		VMM::MapMemory(info->kernelVirtualSpace, PHYSBASE, CONFIG_SYMBOL_TABLE_BASE);// + i);
//	}

	KRNLSYMTABLE = CONFIG_SYMBOL_TABLE_BASE;
	memset(KRNLSYMTABLE, 0, 0x1000);

	KRNLSYMTABLE[KRNLSYMTABLE_PRINTK] = &PRINTK::VPrintK;
}
