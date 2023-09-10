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

VMM::PageList *LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space);
/*void LinkSymbols(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader);*/
size_t LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space, VMM::PageList *elfPages);

uint64_t LoadELF(uint8_t *data, size_t size) {
	VMM::VirtualSpace *space = VMM::NewVirtualSpace();
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	VMM::PageList *elfPages = LoadProgramHeaders(data, size, elfHeader, space);
/*	LinkSymbols(data, size, elfHeader);*/
	size_t pid = LoadProcess(elfHeader, space, elfPages);

	return pid;
}

VMM::PageList *LoadProgramHeaders(uint8_t *data, size_t size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();
	(void)size;

	size_t programHeaderSize = elfHeader->e_phentsize;
	size_t programHeaderOffset = elfHeader->e_phoff;
	size_t programHeaderNumber = elfHeader->e_phnum;

	size_t progSize = 0;

	Elf64_Phdr *programHeader;
	void *lastPhysicalPage = NULL;

	size_t pageCount = 0;

	for (size_t i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				for (uintptr_t addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;) {
					size_t pageAmount = (PAGE_SIZE - addr % PAGE_SIZE);
					addr += pageAmount;
					++pageCount;
				}
				}
				break;

		}
	}

	PRINTK::PrintK("%d pages in list.\r\n", pageCount);
	size_t pageListSize = sizeof(VMM::PageList) + pageCount * sizeof(VMM::PageMetadata);
	VMM::PageList *elfPages = NULL;
	elfPages = (VMM::PageList*)Malloc(pageListSize);
	elfPages->PageCount = pageCount;
	elfPages->AllocatedSize = pageListSize;

	size_t pageSelector = 0;

	for (size_t i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				size_t fileRemaining = programHeader->p_filesz;
				size_t copiedAmount = 0;

				for (uintptr_t addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;) {
					size_t pageAmount = (PAGE_SIZE - addr % PAGE_SIZE);
					size_t copyAmount = fileRemaining > pageAmount ? pageAmount : fileRemaining;

					if(pageSelector >= pageCount) OOPS("pageSelector is higher than pageCount");

					lastPhysicalPage = PMM::RequestPage();
					uintptr_t higher = (uintptr_t)lastPhysicalPage + info->HigherHalfMapping;
					memset((void*)higher, 0, PAGE_SIZE);

					elfPages->Pages[pageSelector].IsCOW = true;
					elfPages->Pages[pageSelector].Data.COW = (VMM::COWMetadata*)Malloc(sizeof(VMM::COWMetadata) + sizeof(uintptr_t));
					elfPages->Pages[pageSelector].Data.COW->PhysicalAddressOfOriginal = (uintptr_t)lastPhysicalPage;
					elfPages->Pages[pageSelector].Data.COW->PhysicalAddressOfCopy = 0;
					elfPages->Pages[pageSelector].Data.COW->VirtualReferences = 1;
					elfPages->Pages[pageSelector].Data.COW->VirtualAddresses[0] = (addr - addr % PAGE_SIZE);

					VMM::MapMemory(space, lastPhysicalPage, (void*)(addr - addr % PAGE_SIZE), VMM::VMM_PRESENT | VMM::VMM_USER);

					if(fileRemaining > 0) {
						memcpy((void*)(higher + addr % PAGE_SIZE),
								(void*)(data + programHeader->p_offset + copiedAmount),
								copyAmount);

						copiedAmount += copyAmount;
						fileRemaining = programHeader->p_filesz - copiedAmount;
					}

					addr += pageAmount;

					++pageSelector;
				}
				}
				break;

		}

		progSize += programHeader->p_memsz;
	}

	PRINTK::PrintK("Loading Complete. Program is %d bytes.\r\n", progSize);

	return elfPages;
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

size_t LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space, VMM::PageList *elfPages) {
	KInfo *info = GetInfo();
	
	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::CreateProcess((PROC::ProcessBase*)info->KernelProcess, PROC::ExecutableUnitType::PT_USER, space, elfPages, 0, 0);
	PROC::UserThread *thread = (PROC::UserThread*)PROC::CreateThread((PROC::ProcessBase*)proc, elfHeader->e_entry, 64 * 1024, 0, 0);

	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, thread);

	PRINTK::PrintK("Process created with PID: 0x%x\r\n", proc->ID);
	
	return proc->ID;
}
