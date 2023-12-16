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

static int LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();

	(void)info;
	(void)size;

	usize programHeaderSize = elfHeader->e_phentsize;
	usize programHeaderOffset = elfHeader->e_phoff;
	usize programHeaderNumber = elfHeader->e_phnum;

	Elf64_Phdr *programHeader;

	for (usize i = 0; i < programHeaderNumber; i++) {
		programHeader = (Elf64_Phdr*)(data + programHeaderOffset + programHeaderSize * i);

		if (programHeader->p_type != PT_LOAD) continue;

		uptr virtAddr = programHeader->p_vaddr;
		usize memSize = programHeader->p_memsz;

		uptr fileOffset = (uptr)data + programHeader->p_offset;
		usize fileSize = programHeader->p_filesz;

		usize flags;

		switch (programHeader->p_flags) {
			case PF_R: /* ROData */
				flags = VMM_FLAGS_USER_RODATA;
				break;
			case PF_R | PF_X: /* Text */
				flags = VMM_FLAGS_USER_CODE;
				break;
			case PF_R | PF_W: /* Data or BSS */
				flags = VMM_FLAGS_USER_DATA;
				break;
			default:
			case PF_R | PF_W | PF_X: /* Generic */
				flags = VMM_FLAGS_USER_GENERIC;
				break;
		}
				
		VMM::VMCopyAlloc(space, virtAddr, memSize, flags, fileOffset, virtAddr, fileSize);
	}

	return 0;
}


static usize LoadProcess(Elf64_Ehdr *elfHeader, VMM::VirtualSpace *space) {
	KInfo *info = GetInfo();

	PROC::Process *proc = PROC::CreateProcess(info->KernelScheduler, PROC::ExecutableUnitType::PT_USER, space);
	PROC::Thread *thread = PROC::CreateThread(info->KernelScheduler, proc, elfHeader->e_entry);
	(void)thread;

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Process created with PID: 0x%x\r\n", proc->ID);
	
	return proc->ID;
}

usize LoadELF(u8 *data, usize size) {
	VMM::VirtualSpace *space = VMM::NewVirtualSpace();
	VMM::PrepareUserVirtualSpace(space);
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	LoadProgramHeaders(data, size, elfHeader, space);
	usize pid = LoadProcess(elfHeader, space);

	return pid;
}

SYMBOL::SymbolPair *ExportSymbolTable(u8 *data, usize size) {
	(void)size;
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;

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

                PRINTK::PrintK(PRINTK_DEBUG "Section: %s\r\n", &sectionStrtabData[sectionHeader->sh_name]);

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

        PRINTK::PrintK(PRINTK_DEBUG "Symbol table: 0x%x\r\n", symtab);
        PRINTK::PrintK(PRINTK_DEBUG "String table: 0x%x\r\n", strtab);
        PRINTK::PrintK(PRINTK_DEBUG "There are %d symbols.\r\n", symbolNumber);

	symbolNumber = 2;

	SYMBOL::SymbolPair *symbolVector = (SYMBOL::SymbolPair*)Malloc(sizeof(SYMBOL::SymbolPair) * (symbolNumber + 1));
	if (symbolVector == NULL) {
		return NULL;
	}

        Elf64_Sym *symbol;

        for (usize i = 0; i < symbolNumber; i++) {
                symbol = (Elf64_Sym*)(data + symtab->sh_offset + symtab->sh_entsize * i);

		symbolVector[i].Address = symbol->st_value;
		symbolVector[i].Name = &strtabData[symbol->st_name];

		PRINTK::PrintK(PRINTK_DEBUG " Symbol: %s at 0x%x\r\n", symbolVector[i].Address, symbolVector[i].Name);
        }

	symbolVector[symbolNumber].Address = -1;
	symbolVector[symbolNumber].Name = NULL;

	return symbolVector;
}
