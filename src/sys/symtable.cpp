#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mkmi.h>

uint64_t *KRNLSYMTABLE;

void SetupSymtable() {
	KInfo *info = GetInfo();

	for (int i = 0; i < CONFIG_SYMBOL_TABLE_PAGES * 0x1000; i += 0x1000) {
		VMM::MapMemory(info->kernelVirtualSpace, PMM::RequestPage(), CONFIG_SYMBOL_TABLE_BASE + i);
	}

	KRNLSYMTABLE = CONFIG_SYMBOL_TABLE_BASE;
	memset(KRNLSYMTABLE, 0, 0x1000);
/*
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIEXECKRNLFUNC] = &MKMI_ExecuteKernelFunction;
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIREGISTERMODL] = &MKMI_RegisterModule;
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIUNREGISTMODL] = &MKMI_UnregisterModule;
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIGETMODULE] = &MKMI_GetModule;
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIBUFFERCREATE] = &MKMI_BufferCreate;
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIBUFFERIOCTL] = &MKMI_BufferIO;
	KRNLSYMTABLE[KRNLSYMTABLE_MKMIBUFFERDELETE] = &MKMI_BufferDelete;
*/
}
