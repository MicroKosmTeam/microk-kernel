#include <init/modules.hpp>
#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mm/string.hpp>
#include <sys/elf.hpp>
#include <cdefs.h>
#include <sys/driver.hpp>
#include <mkmi.hpp>
#include <init/kinfo.hpp>
#include <init/fbsplash.hpp>

uint64_t *KRNLSYMTABLE;

namespace MODULE {
void Init() {
	KInfo *info = GetInfo();
	// Kernel symbol table
	for (int i = 0; i < CONFIG_SYMBOL_TABLE_PAGES * 0x1000; i += 0x1000) {
		VMM::MapMemory(CONFIG_SYMBOL_TABLE_BASE + i, PMM::RequestPage());
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

	PRINTK::PrintK("Available modules: %d\r\n", info->moduleCount);

	for (int i = 0; i < info->moduleCount; i++) {
		if (strcmp(info->modules[i]->cmdline, "MODULE") == 0) {
			PRINTK::PrintK("Kernel module: [ %s %d ]\r\n",
					info->modules[i]->path,
					info->modules[i]->size);

			//PRINTK::PrintK("Loading kernel module...\r\n");
			//LoadELF(info->modules[i]->address, info->modules[i]->size);
		} else if (strcmp(info->modules[i]->cmdline, "SPLASH") == 0) {
			PRINTK::PrintK("MicroKosm splash: [ %s %d ]\r\n",
					info->modules[i]->path,
					info->modules[i]->size);

			PRINTK::PrintK("Printing the splash screen on available framebuffers...\r\n");
			LoadFBSplash(info->modules[i]->address, info->modules[i]->size);
		}

	}

	return;
}
}
