#include <elf.h>
#include <elf64.h>
#include <loader.hpp>
#include <printk.hpp>
#include <memory.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <stdarg.h>
#include <pmm.hpp>
#include <vmm.hpp>
#include <cpu.hpp>
#include <kinfo.hpp>

namespace LOADER {
static bool VerifyELF(Elf64_Ehdr *elfHeader);
static int LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, VirtualSpace *space);
static usize LoadProcess(Elf64_Ehdr *elfHeader, VirtualSpace *space);

usize LoadELF(u8 *data, usize size) {
	VirtualSpace *space = VMM::NewVirtualSpace();
	VMM::PrepareUserVirtualSpace(space);
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	(void)size;
	if(VerifyELF(elfHeader)) {
		PRINTK::PrintK(PRINTK_DEBUG "Valid ELF header.\r\n");

		LoadProgramHeaders(data, size, elfHeader, space);
		usize pid = LoadProcess(elfHeader, space);
		
		return pid;
	} else {
		OOPS("Invalid ELF file");
		return -1;
	}
}

static bool VerifyELF(Elf64_Ehdr *elfHeader) {
	if (elfHeader->e_ident[EI_MAG0] != ELFMAG0 ||
	    elfHeader->e_ident[EI_MAG1] != ELFMAG1 ||
	    elfHeader->e_ident[EI_MAG2] != ELFMAG2 ||
	    elfHeader->e_ident[EI_MAG3] != ELFMAG3) {
		/* Invalid ELF magic */
		return false;
	}

	if (elfHeader->e_ident[EI_CLASS] != ELFCLASS64) {
		/* Invalid machine type */
		return false;
	}

	if (elfHeader->e_type != ET_EXEC) {
		/* Not a classic executble file */
		return false;
	}

	return true;
}

static int LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, VirtualSpace *space) {
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
				flags = VMM_FLAGS_USER | VMM_FLAGS_READ | VMM_FLAGS_NOEXEC;
				break;
			case PF_R | PF_X: /* Text */
				flags = VMM_FLAGS_USER | VMM_FLAGS_READ;
				break;
			case PF_R | PF_W: /* Data or BSS */
				flags = VMM_FLAGS_USER | VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC;
				break;
			default: /* Unknown section, just skip */
				continue;
		}
				
		VMM::VMCopyAlloc(space, virtAddr, memSize, flags, fileOffset, virtAddr, fileSize);
	}

	return 0;
}

static usize LoadProcess(Elf64_Ehdr *elfHeader, VirtualSpace *space) {
	KInfo *info = GetInfo();
/*
	uptr stackAddr = 0x10000000000;
	usize stackSize = 0x100000;

	SchedulerContext *context = (SchedulerContext*)VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	context->IP = elfHeader->e_entry;
	context->SP = stackAddr;
	context->BP = stackAddr;
	context->RFLAGS = 0x202;

	VMM::VMAlloc(space, stackAddr - stackSize, stackSize, VMM_FLAGS_USER_DATA);

	VMM::LoadVirtualSpace(space);
	ARCH::GoToUserspace(context);
*/
/*
	PROC::Process *proc = PROC::CreateProcess(info->KernelScheduler, PROC::ExecutableUnitType::PT_USER, space);
	PROC::Thread *thread = PROC::CreateThread(info->KernelScheduler, proc, elfHeader->e_entry);
	(void)thread;
	PRINTK::PrintK(PRINTK_DEBUG "Process created with PID: 0x%x\r\n", proc->ID);
	
	return proc->ID;
*/
	(void)info;
	(void)elfHeader;
	(void)space;
	return 0;
}
}
