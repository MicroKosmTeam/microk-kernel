#include <elf.h>
#include <elf64.h>
#include <sys/elf.hpp>
#include <sys/printk.hpp>
#include <mm/memory.hpp>
#include <mm/string.hpp>
#include <sys/panic.hpp>
#include <stdarg.h>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <init/kinfo.hpp>
#include <module/modulemanager.hpp>
#include <module/buffer.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

size_t LoadELFCoreModule(uint8_t *data, size_t size);

void LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space);
/*void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader);*/
size_t LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space);

uint64_t LoadELF(uint8_t *data, size_t size) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	VMM::VirtualSpace *space = VMM::NewModuleVirtualSpace();
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	LoadProgramHeaders(data, size, elfHeader, space);
/*	LinkSymbols(data, size, elfHeader);*/
	size_t pid  = LoadProcess(elfHeader, space);

	return pid;
}

void LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();
	(void)size;

	size_t programHeaderSize = elfHeader->e_phentsize;
	size_t programHeaderOffset = elfHeader->e_phoff;
	size_t programHeaderNumber = elfHeader->e_phnum;

	size_t progSize = 0;

	Elf64_Phdr *programHeader;
	for (size_t i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				char buffer[PAGE_SIZE];
				size_t fileRemaining = programHeader->p_filesz;
				uintptr_t virtualAddr = programHeader->p_vaddr;
				size_t memorySize = programHeader->p_memsz;
				size_t fileSize = programHeader->p_filesz;
				VMM::LoadVirtualSpace(info->KernelVirtualSpace);

				for (uintptr_t addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;
				     addr += PAGE_SIZE) {
					VMM::MapMemory(space, PMM::RequestPage(), (void*)addr);
				}

				VMM::LoadVirtualSpace(space);
				memset((void*)virtualAddr, 0, memorySize);
				
				for (size_t i = 0; i < fileSize; i += PAGE_SIZE) {
					if(fileRemaining == 0) break;

					VMM::LoadVirtualSpace(info->KernelVirtualSpace);
					memcpy((void*)buffer, (void*)(data + programHeader->p_offset + i), fileRemaining > PAGE_SIZE ? PAGE_SIZE : fileRemaining);
					
					fileRemaining = programHeader->p_filesz - i;

					VMM::LoadVirtualSpace(space);
					memcpy((void*)(virtualAddr + i), (void*)buffer, fileRemaining > PAGE_SIZE ? PAGE_SIZE : fileRemaining);
				}
					

				VMM::LoadVirtualSpace(info->KernelVirtualSpace);

				}
				break;

		}

		progSize += programHeader->p_memsz;
	}

	PRINTK::PrintK("Loading Complete. Program is %d bytes.\r\n", progSize);
}

/*void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader) {
	size_t sectionHeaderSize = elfHeader->e_shentsize;
	size_t sectionHeaderOffset = elfHeader->e_shoff;
	size_t sectionHeaderNumber = elfHeader->e_shnum;

	Elf64_Shdr *sectionHeader;
	Elf64_Shdr *symtab = NULL;
	Elf64_Shdr *strtab = NULL;

	Elf64_Shdr *sectionStrtab = (Elf64_Shdr*)(data + sectionHeaderOffset + sectionHeaderSize * elfHeader->e_shstrndx);
	const char *sectionStrtabData = (const char*)(data + sectionStrtab->sh_offset);

	for (size_t i = 0; i < sectionHeaderNumber; i++) {
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

	for (size_t i = 0; i < symbolNumber; i++) {
		symbol = (Elf64_Sym*)(data + symtab->sh_offset + symtab->sh_entsize * i);
		const char *name = &strtabData[symbol->st_name];
	}
}*/

size_t LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::CreateProcess((PROC::ProcessBase*)info->KernelProcess, PROC::ExecutableUnitType::PT_USER, space, 0, 0);
	PROC::UserThread *thread = (PROC::UserThread*)PROC::CreateThread((PROC::ProcessBase*)proc, elfHeader->e_entry, 64 * 1024, 0, 0);

	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, thread);
	
	/* Cleaning up */
	if((void*)elfHeader > (void*)CONFIG_HEAP_BASE) Free(elfHeader);

	return proc->ID;
}
