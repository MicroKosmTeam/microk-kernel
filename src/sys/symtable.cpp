#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mkmi.hpp>

uint64_t *KRNLSYMTABLE;

void SetupSymtable() {
	KInfo *info = GetInfo();

	for (int i = 0; i < CONFIG_SYMBOL_TABLE_PAGES * 0x1000; i += 0x1000) {
		VMM::MapMemory(info->kernelVirtualSpace, PMM::RequestPage(), CONFIG_SYMBOL_TABLE_BASE + i);
	}

	KRNLSYMTABLE = CONFIG_SYMBOL_TABLE_BASE;
	memset(KRNLSYMTABLE, 0, 0x1000);

	KRNLSYMTABLE[KRNLSYMTABLE_REQUESTPAGE] = &PMM::RequestPage;
	KRNLSYMTABLE[KRNLSYMTABLE_REQUESTPAGES] = &PMM::RequestPages;
	KRNLSYMTABLE[KRNLSYMTABLE_MEMCPY] = &memcpy;
	KRNLSYMTABLE[KRNLSYMTABLE_MEMSET] = &memset;
	KRNLSYMTABLE[KRNLSYMTABLE_MEMCMP] = &memcmp;
	KRNLSYMTABLE[KRNLSYMTABLE_PRINTK] = &PRINTK::PrintK;
	KRNLSYMTABLE[KRNLSYMTABLE_MALLOC] = &Malloc;
	KRNLSYMTABLE[KRNLSYMTABLE_FREE] = &Free;
	KRNLSYMTABLE[KRNLSYMTABLE_STRCPY] = &strcpy;
	KRNLSYMTABLE[KRNLSYMTABLE_BUFFERCREATE] = &MKMI::BufferCreate;
	KRNLSYMTABLE[KRNLSYMTABLE_BUFFERIOCTL] = &MKMI::BufferIO;
	KRNLSYMTABLE[KRNLSYMTABLE_BUFFERDELETE] = &MKMI::BufferDelete;
}
