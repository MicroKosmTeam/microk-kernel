#include <init/modules.hpp>

#ifdef CONFIG_KERNEL_MODULES

#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <sys/elf.hpp>
#include <mm/vmm.hpp>
#include <sys/driver.hpp>
#include <init/kinfo.hpp>
#include <sys/user.hpp>
#include <sys/symtable.hpp>
#include <mkmi.h>
#include <module/modulemanager.hpp>

namespace MODULE {

void Init() {
	SetupSymtable();
	Manager::Init();

	PRINTK::PrintK("Module managment initialized.\r\n");

	KInfo *info = GetInfo();

	PRINTK::PrintK("Available modules: %d\r\n", info->moduleCount);

	for (int i = 0; i < info->moduleCount; i++) {
		if (strcmp(info->modules[i].cmdline, "MODULE") == 0) {
			PRINTK::PrintK("Kernel module: [ %s %d ]\r\n",
					info->modules[i].path,
					info->modules[i].size);

			PRINTK::PrintK("Loading kernel module at 0x%x...\r\n", info->modules[i].address);

			LoadELF(ELF_CORE_MODULE, info->modules[i].address, info->modules[i].size);
		} else if (strcmp(info->modules[i].cmdline, "MKMI") == 0) {
			PRINTK::PrintK("MKMI: [ %s %d ]\r\n",
					info->modules[i].path,
					info->modules[i].size);

			PRINTK::PrintK("Loading and initializing MKMI at 0x%x...\r\n", info->modules[i].address);

			LoadELF(ELF_MKMI, info->modules[i].address, info->modules[i].size);
		} else {
			PRINTK::PrintK("Unknown file: [ %s %d ]\r\n",
					info->modules[i].path,
					info->modules[i].size);
		}
	}
}
}

#endif
