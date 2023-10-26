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
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

usize LoadELFCoreModule(u8 *data, usize size);

VMM::PageList *LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, uptr space);
/*void LinkSymbols(u8 *data, usize size, Elf64_Ehdr *elfHeader);*/
usize LoadProcess(Elf64_Ehdr *elfHeader, uptr space, VMM::PageList *elfPages);

u64 LoadELF(u8 *data, usize size) {
	uptr space = VMM::NewVirtualSpace();
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	VMM::PageList *elfPages = LoadProgramHeaders(data, size, elfHeader, space);
/*	LinkSymbols(data, size, elfHeader);*/
	usize pid = LoadProcess(elfHeader, space, elfPages);

	return pid;
}

VMM::PageList *LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, uptr space) {
	KInfo *info = GetInfo();
	(void)size;

	usize programHeaderSize = elfHeader->e_phentsize;
	usize programHeaderOffset = elfHeader->e_phoff;
	usize programHeaderNumber = elfHeader->e_phnum;

	usize progSize = 0;

	Elf64_Phdr *programHeader;
	uptr lastPhysicalPage = 0;

	usize pageCount = 0;

	for (usize i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				for (uptr addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;) {
					usize pageAmount = (PAGE_SIZE - addr % PAGE_SIZE);
					addr += pageAmount;
					++pageCount;
				}
				}
				break;

		}
	}

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "%d pages in list.\r\n", pageCount);
	usize pageListSize = sizeof(VMM::PageList) + pageCount * sizeof(VMM::PageMetadata);
	VMM::PageList *elfPages = NULL;
	elfPages = (VMM::PageList*)Malloc(pageListSize);
	elfPages->PageCount = pageCount;
	elfPages->AllocatedSize = pageListSize;

	usize pageSelector = 0;

	for (usize i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);
		if(programHeader->p_memsz == 0) continue;

		switch (programHeader->p_type) {
			case PT_LOAD: {
				usize fileRemaining = programHeader->p_filesz;
				usize copiedAmount = 0;

				for (uptr addr = programHeader->p_vaddr;
				     addr < programHeader->p_vaddr + programHeader->p_memsz;) {
					usize pageAmount = (PAGE_SIZE - addr % PAGE_SIZE);
					usize copyAmount = fileRemaining > pageAmount ? pageAmount : fileRemaining;

					if(pageSelector >= pageCount) OOPS("pageSelector is higher than pageCount");

					lastPhysicalPage = (uptr)PMM::RequestPage();
					uptr higher = lastPhysicalPage + info->HigherHalfMapping;
					Memset((void*)higher, 0, PAGE_SIZE);

					elfPages->Pages[pageSelector].IsCOW = true;
					elfPages->Pages[pageSelector].Data.COW = (VMM::COWMetadata*)Malloc(sizeof(VMM::COWMetadata) + sizeof(uptr));
					elfPages->Pages[pageSelector].Data.COW->PhysicalAddressOfOriginal = (uptr)lastPhysicalPage;
					elfPages->Pages[pageSelector].Data.COW->PhysicalAddressOfCopy = 0;
					elfPages->Pages[pageSelector].Data.COW->VirtualReferences = 1;
					elfPages->Pages[pageSelector].Data.COW->VirtualAddresses[0] = (addr - addr % PAGE_SIZE);

					VMM::MapPage(space, lastPhysicalPage, addr - addr % PAGE_SIZE, VMM_FLAGS_USER_CODE);

					if(fileRemaining > 0) {
						Memcpy((void*)(higher + addr % PAGE_SIZE),
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

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Loading Complete. Program is %d bytes.\r\n", progSize);

	return elfPages;
}

/*void LinkSymbols(u8 *data, usize size, Elf64_Ehdr *elfHeader) {
	usize sectionHeaderSize = elfHeader->e_shentsize;
	usize sectionHeaderOffset = elfHeader->e_shoff;
	usize sectionHeaderNumber = elfHeader->e_shnum;

	Elf64_Shdr *sectionHeader;
	Elf64_Shdr *symtab = NULL;
	Elf64_Shdr *strtab = NULL;

	Elf64_Shdr *sectionStrtab = (Elf64_Shdr*)(data + sectionHeaderOffset + sectionHeaderSize * elfHeader->e_shstrndx);
	const char *sectionStrtabData = (const char*)(data + sectionStrtab->sh_offset);

	for (usize i = 0; i < sectionHeaderNumber; i++) {
		sectionHeader = (Elf64_Shdr*)(data + sectionHeaderOffset + sectionHeaderSize * i);

		//PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Section: %s\r\n", &sectionStrtabData[sectionHeader->sh_name]);

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

	usize symbolNumber = symtab->sh_size / symtab->sh_entsize;
	const char *strtabData = (const char*)(data + strtab->sh_offset);

	//PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Symbol table: 0x%x\r\n", symtab);
	//PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "There are %d symbols.\r\n", symbolNumber);

	Elf64_Sym *symbol;

	for (usize i = 0; i < symbolNumber; i++) {
		symbol = (Elf64_Sym*)(data + symtab->sh_offset + symtab->sh_entsize * i);
		const char *name = &strtabData[symbol->st_name];
	}
}*/

usize LoadProcess(Elf64_Ehdr *elfHeader, uptr space, VMM::PageList *elfPages) {
	KInfo *info = GetInfo();
	
	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::CreateProcess((PROC::ProcessBase*)info->KernelProcess, PROC::ExecutableUnitType::PT_USER, space, elfPages, 0, 0);
	PROC::UserThread *thread = (PROC::UserThread*)PROC::CreateThread((PROC::ProcessBase*)proc, elfHeader->e_entry, 64 * 1024, 0, 0);

	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, thread);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Process created with PID: 0x%x\r\n", proc->ID);
	
	return proc->ID;
}
