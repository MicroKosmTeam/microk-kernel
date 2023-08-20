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
#include <init/kinfo.hpp>
#include <module/modulemanager.hpp>
#include <module/buffer.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

size_t LoadELFCoreModule(uint8_t *data, size_t size);

void LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space);
void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader);
size_t LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space);

uint64_t LoadELF(uint8_t *data, size_t size) {
	KInfo *info = GetInfo();

	VMM::VirtualSpace *space = VMM::NewModuleVirtualSpace();
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	LoadProgramHeaders(data, size, elfHeader, space);
	LinkSymbols(data, size, elfHeader);
	size_t pid  = LoadProcess(elfHeader, space);

	return pid;
}

void LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();

	uint64_t programHeaderSize = elfHeader->e_phentsize;
	uint64_t programHeaderOffset = elfHeader->e_phoff;
	uint64_t programHeaderNumber = elfHeader->e_phnum;

	uint64_t progSize = 0;

	Elf64_Phdr *programHeader;
	for (int currentHeader = 0; currentHeader < programHeaderNumber; currentHeader++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * currentHeader);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				size_t pageAmount = programHeader->p_memsz / PAGE_SIZE + 1;
				uintptr_t *pages = new uintptr_t[pageAmount];
				memset(pages, 0, pageAmount * sizeof(uintptr_t));

				size_t fileRemaining = programHeader->p_filesz;

				for (size_t page = 0; page < pageAmount; ++page) {
					uintptr_t virtualAddr = programHeader->p_vaddr + page * PAGE_SIZE;

					uintptr_t addr = space->GetPhysicalAddress((void*)virtualAddr);
					PRINTK::PrintK("Addr: 0x%x -> 0x%x\r\n", virtualAddr, addr);
					if (addr == NULL || addr == virtualAddr) {
						pages[page] = PMM::RequestPage();

						VMM::MapMemory(space, pages[page], virtualAddr);
						memset(pages[page], 0, PAGE_SIZE);
					} else pages[page] = addr;
				}

				for (size_t page = 0; page * PAGE_SIZE < programHeader->p_filesz; ++page) {
					size_t offset = page * PAGE_SIZE;

					PRINTK::PrintK("Page address: 0x%x\r\n", pages[page]);
					memcpy(pages[page], data + programHeader->p_offset + offset, fileRemaining > PAGE_SIZE ? PAGE_SIZE : fileRemaining);
					
					fileRemaining = programHeader->p_filesz - offset;
				}
					
				delete[] pages;
				}
				break;

		}

		progSize += programHeader->p_memsz;
	}

	PRINTK::PrintK("Loading Complete. Program is %d bytes.\r\n", progSize);
}

void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader) {
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

	for (int i = 0; i < symbolNumber; i++) {
		symbol = (Elf64_Sym*)(data + symtab->sh_offset + symtab->sh_entsize * i);
		const char *name = &strtabData[symbol->st_name];
	}
}

size_t LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();

	PROC::Process *proc = new PROC::Process(PROC::PT_USER, space);

	size_t pid = proc->GetPID();
	size_t tid = proc->CreateThread(65536, elfHeader->e_entry);

	proc->SetMainThread(tid);
	PROC::Thread *mainThread = proc->GetThread(tid);

	info->kernelScheduler->AddProcess(proc);

	return pid;
}
