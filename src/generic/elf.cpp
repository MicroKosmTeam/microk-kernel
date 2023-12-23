#include <elf.h>
#include <elf64.h>
#include <elf.hpp>
#include <printk.hpp>
#include <memory.hpp>
#include <string.hpp>
#include <panic.hpp>
#include <stdarg.h>
#include <pmm.hpp>
#include <vmm.hpp>
#include <kinfo.hpp>

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

usize LoadELF(u8 *data, usize size) {
	VMM::VirtualSpace *space = VMM::NewVirtualSpace();
	VMM::PrepareUserVirtualSpace(space);
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	LoadProgramHeaders(data, size, elfHeader, space);
	usize pid = LoadProcess(elfHeader, space);

	return pid;
}
