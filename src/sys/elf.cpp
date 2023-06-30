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
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

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

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	VMM::VirtualSpace *space = VMM::NewModuleVirtualSpace();
	PROC::Process *module = new PROC::Process(PROC::PT_USER, space);
	
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

	VMM::VirtualSpace *space = proc->GetVirtualMemorySpace();

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
				char buffer[1024];
				size_t fileRemaining = programHeader->p_filesz;
				uintptr_t virtualAddr = programHeader->p_vaddr;
				size_t memorySize = programHeader->p_memsz;
				size_t fileSize = programHeader->p_filesz;
				VMM::LoadVirtualSpace(info->kernelVirtualSpace);

				for (uint64_t addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;
				     addr+=0x1000) {
					VMM::MapMemory(proc->GetVirtualMemorySpace(), PMM::RequestPage(), addr);
				}

				
				PRINTK::PrintK("Create data...\r\n");

				VMM::LoadVirtualSpace(space);
				memset(virtualAddr, 0, memorySize);
				
				for (size_t i = 0; i < fileSize; i += 1024) {
					if(fileRemaining == 0) break;

					VMM::LoadVirtualSpace(info->kernelVirtualSpace);
					memcpy(buffer, data + programHeader->p_offset + i, fileRemaining > 1024 ? 1024 : fileRemaining);
					
					fileRemaining = programHeader->p_filesz - i;

					VMM::LoadVirtualSpace(space);
					memcpy(virtualAddr + i, buffer, fileRemaining > 1024 ? 1024 : fileRemaining);
				}
					

				VMM::LoadVirtualSpace(info->kernelVirtualSpace);

				}
				break;

		}

		progSize += programHeader->p_memsz;
	}

	PRINTK::PrintK("Loading Complete. Program is %d bytes.\r\n", progSize);
}

void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, PROC::Process *proc) {
	KInfo *info = GetInfo();
				
	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

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
	MODULE::Manager::RegisterModule(modinfo, proc);
	VMM::LoadVirtualSpace(proc->GetVirtualMemorySpace());
}

void LoadProcess(Elf64_Ehdr *elfHeader, PROC::Process *proc) {
	KInfo *info = GetInfo();

	PRINTK::PrintK("Creating thread..\r\n");
	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	if(elfHeader > CONFIG_HEAP_BASE) Free(elfHeader);

	size_t pid = proc->GetPID();
	size_t tid = proc->CreateThread(0, elfHeader->e_entry);

	proc->SetMainThread(tid);
	PROC::Thread *mainThread = proc->GetThread(tid);

	PRINTK::PrintK("Created thread %d for process %d.\r\n", tid, pid);

	info->kernelScheduler->AddProcess(proc);
	PRINTK::PrintK("Switching to task...\r\n");
	info->kernelScheduler->SwitchToTask(pid, 0);

	//MODULE::Manager::UnregisterModule(modinfo->ID);
}
