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

uint64_t LoadELF(uint8_t *data, size_t size) {
	/* Checking for the correct signature */
	if (data[0] != 0x7F || data[1] != 'E' || data[2] != 'L' || data[3] != 'F') {
		return NULL;
	}

	/* Getting important kernel information */
	KInfo *info = GetInfo();

	/* Initializing the ELF header */
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;

	/* Checking if the file is valid for the current architecture */
	if(elfHeader->e_ident[EI_CLASS] != ELFCLASS64) {
		return NULL;
	}

	if(elfHeader->e_type != ET_DYN) {
		return NULL;
	}

	uint64_t programHeaderSize = elfHeader->e_phentsize;
	uint64_t programHeaderOffset = elfHeader->e_phoff;
	uint64_t programHeaderNumber = elfHeader->e_phnum;

	uint64_t progSize = 0;

	Elf64_Phdr *programHeader;
	for (int i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);

		if(programHeader->p_type == PT_LOAD && programHeader->p_memsz > 0) {
			for (uint64_t addr = programHeader->p_vaddr; addr < programHeader->p_vaddr + programHeader->p_memsz; addr+=0x1000) {
				VMM::MapMemory(info->kernelVirtualSpace, PMM::RequestPage(), addr);
			}

			memset(programHeader->p_vaddr, 0, programHeader->p_memsz);
			memcpy(programHeader->p_vaddr, data + programHeader->p_offset, programHeader->p_filesz);
		}

		progSize += programHeader->p_memsz;
	}


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

		PRINTK::PrintK("Section: %s\r\n", &sectionStrtabData[sectionHeader->sh_name]);

		switch(sectionHeader->sh_type) {
			case SHT_PROGBITS:
				break;
			case SHT_SYMTAB:
				symtab = sectionHeader;
				break;
			case SHT_STRTAB:
				if (elfHeader->e_shstrndx == i) break;
				strtab = sectionHeader;
				break;
			case SHT_DYNSYM:
				break;
			case SHT_NOBITS:
				break;
			default:
				break;
		}
	}
	size_t symbolNumber = symtab->sh_size / symtab->sh_entsize;
	const char *strtabData = (const char*)(data + strtab->sh_offset);

	PRINTK::PrintK("Symbol table: 0x%x\r\n", symtab);
	PRINTK::PrintK("There are %d symbols.\r\n", symbolNumber);

	Elf64_Sym *symbol;
	MKMI_Module *modinfo;

	uint64_t (*entry)();
	uint64_t (*exit)();

	for (int i = 0; i < symbolNumber; i++) {
		symbol = (Elf64_Sym*)(data + symtab->sh_offset + symtab->sh_entsize * i);
		const char *name = &strtabData[symbol->st_name];

		PRINTK::PrintK("-> %s at 0x%x\r\n", name, symbol->st_value);

		if (strcmp(name, "ModuleInfo") == 0) {
			modinfo = symbol->st_value;
			PRINTK::PrintK("Module info:\r\n"
					" -> Module name: %s 0x%x\r\n"
					" -> Author name: %s 0x%x\r\n",
					modinfo->ID.Name, modinfo->ID.Codename,
					modinfo->ID.Author, modinfo->ID.Writer);
		} else if (strcmp(name, "ModuleInit") == 0) {
			entry = symbol->st_value;
		} else if (strcmp(name, "ModuleDeinit") == 0) {
			exit = symbol->st_value;
		}
	}


	PRINTK::PrintK("Loading Complete. Program is %d bytes.\r\n", progSize);
	
	PRINTK::PrintK("Result: %d\r\n", entry());

	return NULL;
}
