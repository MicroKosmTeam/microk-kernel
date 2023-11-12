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

int LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, uptr space);
/*void LinkSymbols(u8 *data, usize size, Elf64_Ehdr *elfHeader);*/
usize LoadProcess(Elf64_Ehdr *elfHeader, uptr space, VMM::PageList *elfPages);

u64 LoadELF(u8 *data, usize size) {
	uptr space = VMM::NewVirtualSpace();
	VMM::PrepareUserVirtualSpace(space);
	
	Elf64_Ehdr *elfHeader = (Elf64_Ehdr*)data;
	
	VMM::PageList *elfPages = NULL;
	LoadProgramHeaders(data, size, elfHeader, space);
/*	LinkSymbols(data, size, elfHeader);*/
	usize pid = LoadProcess(elfHeader, space, elfPages);

	return pid;
}

int LoadProgramHeaders(u8 *data, usize size, Elf64_Ehdr *elfHeader, uptr space) {
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

usize LoadProcess(Elf64_Ehdr *elfHeader, uptr space, VMM::PageList *elfPages) {
	KInfo *info = GetInfo();
	
	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::CreateProcess((PROC::ProcessBase*)info->KernelProcess, PROC::ExecutableUnitType::PT_USER, space, elfPages, 0, 0);
	PROC::UserThread *thread = (PROC::UserThread*)PROC::CreateThread((PROC::ProcessBase*)proc, elfHeader->e_entry, 64 * 1024, 0, 0);

	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, thread);

	PRINTK::PrintK(PRINTK_DEBUG MODULE_NAME "Process created with PID: 0x%x\r\n", proc->ID);
	
	return proc->ID;
}
