#include <init/modules.hpp>
#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <sys/elf.hpp>
#include <cdefs.h>
#include <sys/driver.hpp>
#include <init/kinfo.hpp>
#include <init/fbsplash.hpp>
#include <sys/user.hpp>
#include <sys/symtable.hpp>
#include <mkmi.hpp>

namespace MODULE {

void Init() {
	SetupSymtable();

	KInfo *info = GetInfo();

	PRINTK::PrintK("Available modules: %d\r\n", info->moduleCount);

	for (int i = 0; i < info->moduleCount; i++) {
		if (strcmp(info->modules[i].cmdline, "MODULE") == 0) {
			PRINTK::PrintK("Kernel module: [ %s %d ]\r\n",
					info->modules[i].path,
					info->modules[i].size);

			//PRINTK::PrintK("Loading kernel module...\r\n");
			//void *function = LoadELF(info->modules[i].address, info->modules[i].size);
			//EnterUserspace(function, PMM::RequestPage());
		} else if (strcmp(info->modules[i].cmdline, "SPLASH") == 0) {
			PRINTK::PrintK("MicroKosm splash: [ %s %d ]\r\n",
					info->modules[i].path,
					info->modules[i].size);

			//PRINTK::PrintK("Printing the splash screen on available framebuffers...\r\n");
			//LoadFBSplash(info->modules[i].address, info->modules[i].size);
		} else {
			PRINTK::PrintK("Unknown file: [ %s %d ]\r\n",
					info->modules[i].path,
					info->modules[i].size);
		}
	}

	return;
}
}
