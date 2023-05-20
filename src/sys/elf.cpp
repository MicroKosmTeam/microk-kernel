#include <elf.h>
#include <elf64.h>
#include <sys/elf.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <sys/panic.hpp>
#include <stdarg.h>
#include <sys/driver.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mkmi.h>
#include <init/kinfo.hpp>
#include <module/modulemanager.hpp>
#include <module/buffer.hpp>
#include <sys/user.hpp>
#include <proc/process.hpp>

void LoadMKMI(uint8_t *data, size_t size);
void LoadELFCoreModule(uint8_t *data, size_t size);

void LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, PROC::Process *proc);
void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, PROC::Process *proc);
void LoadProcess(Elf64_Ehdr *elfHeader, PROC::Process *proc);

uint64_t LoadELF(ELFType type, uint8_t *data, size_t size) {
	/* Checking for the correct signature */
	if (data[0] != 0x7F || data[1] != 'E' || data[2] != 'L' || data[3] != 'F') {
		return -1;
	}

	switch (type) {
		case ELF_MKMI:
			PRINTK::PrintK("Loading MKMI...\r\n");
			LoadMKMI(data, size);
			break;

		case ELF_CORE_MODULE:
			PRINTK::PrintK("Loading core module...\r\n");
			LoadELFCoreModule(data, size);
			break;
	}

	return 0;
}

void LoadMKMI(uint8_t *data, size_t size) {
	KInfo *info = GetInfo();
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;

	/* Checking if the file is valid for the current architecture */
	if(elfHeader->e_ident[EI_CLASS] != ELFCLASS64) {
		return NULL;
	}

	// Do dynamic loading stuff
}

void LoadELFCoreModule(uint8_t *data, size_t size) {
	KInfo *info = GetInfo();

	VMM::VirtualSpace *space = VMM::NewModuleVirtualSpace();
	PROC::Process *module = PROC::CreateProcess(PROC::PT_USER, space);

	VMM::LoadVirtualSpace(module->VirtualMemorySpace);
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;

	/* Checking if the file is valid for the current architecture */
	if(elfHeader->e_ident[EI_CLASS] != ELFCLASS64) {
		return NULL;
	}

	LoadProgramHeaders(data, size, elfHeader, module);
	LinkSymbols(data, size, elfHeader, module);
	LoadProcess(elfHeader, module);
}

void LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, PROC::Process *proc) {
	KInfo *info = GetInfo();

	uint64_t programHeaderSize = elfHeader->e_phentsize;
	uint64_t programHeaderOffset = elfHeader->e_phoff;
	uint64_t programHeaderNumber = elfHeader->e_phnum;

	uint64_t progSize = 0;

	Elf64_Phdr *programHeader;
	for (int i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				VMM::LoadVirtualSpace(info->kernelVirtualSpace);

				for (uint64_t addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;
				     addr+=0x1000) {
					VMM::MapMemory(proc->VirtualMemorySpace, PMM::RequestPage(), addr);
				}

				VMM::LoadVirtualSpace(proc->VirtualMemorySpace);

				memset(programHeader->p_vaddr, 0, programHeader->p_memsz);
				memcpy(programHeader->p_vaddr, data + programHeader->p_offset, programHeader->p_filesz);
				}
				break;

		}

		progSize += programHeader->p_memsz;
	}

	PRINTK::PrintK("Loading Complete. Program is %d bytes.\r\n", progSize);
}

void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, PROC::Process *proc) {
	KInfo *info = GetInfo();

	uint64_t sectionHeaderSize = elfHeader->e_shentsize;
	uint64_t sectionHeaderOffset = elfHeader->e_shoff;
	uint64_t sectionHeaderNumber = elfHeader->e_shnum;

	Elf64_Shdr *sectionHeader;
	Elf64_Shdr *symtab = NULL;
	Elf64_Shdr *strtab = NULL;

	Elf64_Shdr *sectionStrtab = (Elf64_Shdr*)(data + sectionHeaderOffset + sectionHeaderSize * elfHeader->e_shstrndx);
	const char *sectionStrtabData = (const char*)(data + sectionStrtab->sh_offset);

	for (int i = 0; i < sectionHeaderNumber; i++) {
		sectionHeader = (Elf64_Shdr*)(data + sectionHeaderOffset + sectionHeaderSize * i);

		//PRINTK::PrintK("Section: %s\r\n", &sectionStrtabData[sectionHeader->sh_name]);

		switch(sectionHeader->sh_type) {
			case SHT_SYMTAB: {
				symtab = sectionHeader;
				}
				break;
			case SHT_STRTAB: {
				if (elfHeader->e_shstrndx == i) break;
				strtab = sectionHeader;
				}
				break;
		}
	}

	size_t symbolNumber = symtab->sh_size / symtab->sh_entsize;
	const char *strtabData = (const char*)(data + strtab->sh_offset);

	//PRINTK::PrintK("Symbol table: 0x%x\r\n", symtab);
	//PRINTK::PrintK("There are %d symbols.\r\n", symbolNumber);

	Elf64_Sym *symbol;
	MKMI_Module modinfo;

	for (int i = 0; i < symbolNumber; i++) {
		symbol = (Elf64_Sym*)(data + symtab->sh_offset + symtab->sh_entsize * i);
		const char *name = &strtabData[symbol->st_name];

		if (strcmp(name, "ModuleInfo") == 0) {
			memcpy(&modinfo, (MKMI_Module*)(symbol->st_value), sizeof(MKMI_Module));
		}
	}

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	MODULE::Manager::RegisterModule(modinfo);
	VMM::LoadVirtualSpace(proc->VirtualMemorySpace);
}

#include <arch/x64/cpu/stack.hpp>
void LoadProcess(Elf64_Ehdr *elfHeader, PROC::Process *proc) {
	KInfo *info = GetInfo();
	PRINTK::PrintK("Creating thread..\r\n");
	void *entry = elfHeader->e_entry;
	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	PROC::Thread *mainThread = PROC::CreateThread(proc, entry);
	VMM::LoadVirtualSpace(proc->VirtualMemorySpace);
	
	/*SaveContext *context = mainThread->Stack - sizeof(SaveContext);
	PRINTK::PrintK("Save context: 0x%x\r\n"
			"RBP: 0x%x\r\n"
			"RBP2: 0x%x\r\n"
			"ret: 0x%x\r\n", 
			context,
			context->RBP, context->RBP2, context->ret);*/
	PRINTK::PrintK("Launching...\r\n");

	void *stack = mainThread->Stack + sizeof(SaveContext);
	PRINTK::PrintK("Switching to userspace.\r\n"
	               " Function Address: 0x%x\r\n"
		       " Stack Address:    0x%x\r\n",
		       (uint64_t)entry,
		       (uint64_t)stack);
	EnterUserspace(entry, stack);

	//SwitchStack(&info->kernelStack, &mainThread->Stack);
	
	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	//MODULE::Manager::UnregisterModule(modinfo->ID);

}
