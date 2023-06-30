#include <sys/syscall.hpp>
#include <stdint.h>
#include <stddef.h>
#include <cdefs.h>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>

// TMP measure: do something better with SMP
__attribute__((__aligned__((16)))) __attribute__((section(".syscall.stack"))) volatile char SyscallStack[128 * 1024];
__attribute__((section(".syscall.stack"))) __attribute__((__aligned__((16)))) extern "C" void *StartSyscallStack = &SyscallStack[128 * 1024 - 1];

void AddOverride(size_t syscallNumber);
size_t CheckOverride(size_t syscallNumber);
void RunOverride(size_t syscallNumber);

/* Kernel syscall handlers */
void HandleSyscallDebugPrintK(const char *string);

void HandleSyscallMemoryGetinfo(uintptr_t structbase);
void HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags);
void HandleSyscallMemoryVmfree(uintptr_t base, size_t length);
void HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags);
void HandleSyscallMemoryUnmap(uintptr_t base, size_t length);

void HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize);
void HandleSyscallProcFork(size_t TODO);
void HandleSyscallProcReturn(size_t returnCode, uintptr_t stack);
void HandleSyscallProcExit(size_t exitCode, uintptr_t stack);
void HandleSyscallProcWait(size_t TODO);
void HandleSyscallProcKill(size_t TODO);

void HandleSyscallModuleRegister(size_t TODO);
void HandleSyscallModuleUnregister(size_t TODO);

void HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length);
void HandleSyscallFileRead(size_t TODO);
void HandleSyscallFileWrite(size_t TODO);
void HandleSyscallFileClose(size_t TODO);

void HandleSyscallKernOverride(size_t TODO);

extern "C" void HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6) {
/*	PRINTK::PrintK("\r\n!!! SYSCALL !!!\r\n"
			"RAX: 0x%x\r\n"
			"RDI: 0x%x\r\n"
			"RSI: 0x%x\r\n"
			"RDX: 0x%x\r\n"
			"RCX: 0x%x\r\n"
			"R8:  0x%x\r\n"
			"R9:  0x%x\r\n",
			syscallNumber, arg1, arg2, arg3, arg4, arg5, arg6);*/

	/* Check first if the syscall has been overridden. */
	size_t override = CheckOverride(syscallNumber);
	if(override != 0) return RunOverride(override);

	/* The syscall was not overridden, execute the normal kernel call */
	switch(syscallNumber) {
		case SYSCALL_DEBUG_PRINTK: return HandleSyscallDebugPrintK(arg1);

		case SYSCALL_MEMORY_GETINFO: return HandleSyscallMemoryGetinfo(arg1);
		case SYSCALL_MEMORY_VMALLOC: return HandleSyscallMemoryVmalloc(arg1, arg2, arg3);
		case SYSCALL_MEMORY_VMFREE: return HandleSyscallMemoryVmfree(arg1, arg2);
		case SYSCALL_MEMORY_MMAP: return HandleSyscallMemoryMmap(arg1, arg2, arg3, arg4);
		case SYSCALL_MEMORY_UNMAP: return HandleSyscallMemoryUnmap(arg1, arg2);

		case SYSCALL_PROC_EXEC: return HandleSyscallProcExec(arg1, arg2);
		case SYSCALL_PROC_FORK: return HandleSyscallProcFork(0);
		case SYSCALL_PROC_RETURN: return HandleSyscallProcReturn(arg1, arg2);
		case SYSCALL_PROC_EXIT: return HandleSyscallProcExit(arg1, arg2);
		case SYSCALL_PROC_WAIT: return HandleSyscallProcWait(0);
		case SYSCALL_PROC_KILL: return HandleSyscallProcKill(0);

		case SYSCALL_MODULE_REGISTER: return HandleSyscallModuleRegister(0);
		case SYSCALL_MODULE_UNREGISTER: return HandleSyscallModuleUnregister(0);

		case SYSCALL_FILE_OPEN: return HandleSyscallFileOpen(arg1, arg2, arg3);
		case SYSCALL_FILE_READ: return HandleSyscallFileRead(0);
		case SYSCALL_FILE_WRITE: return HandleSyscallFileWrite(0);
		case SYSCALL_FILE_CLOSE: return HandleSyscallFileClose(0);

		case SYSCALL_KERN_OVERRIDE: return HandleSyscallKernOverride(0);

		default: return 0;
	}
}

void AddOverride(size_t syscallNumber) { return 0; }
size_t CheckOverride(size_t syscallNumber) { return 0; }
void RunOverride(size_t syscallNumber) { return 0; }


void HandleSyscallDebugPrintK(const char *string) {
	PRINTK::PrintK("%s", string);
}

void HandleSyscallMemoryGetinfo(uintptr_t structbase) {
	if (structbase <= 0x1000 || structbase >= 0x00007FFFFFFFFFFF)
		return; /* Make sure it is in valid memory */

	size_t *data = structbase;
	data[0] = PMM::GetFreeMem() + PMM::GetUsedMem();  /* Total */
	data[1] = PMM::GetFreeMem(); /* Free */
	data[2] = PMM::GetUsedMem(); /* Reserved (todo, get correct amount) */
	data[3] = 0; /* Buffers */
}

void HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		size_t *paddr = PMM::RequestPage();
		if (paddr == NULL) PANIC("Out of memory");

		procSpace->MapMemory(paddr, vaddr, flags);
	}

	VMM::LoadVirtualSpace(procSpace);
}

void HandleSyscallMemoryVmfree(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		size_t *paddr = procSpace->GetPhysicalAddress(vaddr);
		procSpace->UnmapMemory(vaddr);
		if (paddr == NULL) return;

		PMM::FreePage(paddr);
	}

	VMM::LoadVirtualSpace(procSpace);
}

void HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags) {
	KInfo *info = GetInfo();
	
	if (src > info->higherHalfMapping) src -= info->higherHalfMapping;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (src % PAGE_SIZE) src -= src % PAGE_SIZE;
	if (dest % PAGE_SIZE) dest -= dest % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	uintptr_t end = src + length;
	for (; src < end; src += PAGE_SIZE, dest += PAGE_SIZE) {
		procSpace->MapMemory(src, dest, flags);
	}

	VMM::LoadVirtualSpace(procSpace);
}

void HandleSyscallMemoryUnmap(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return; /* Make sure it is in valid memory */

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t end = base + length; base < end; base += PAGE_SIZE) {
		procSpace->UnmapMemory(base);
	}

	VMM::LoadVirtualSpace(procSpace);
}

#include <sys/elf.hpp>
#include <sys/printk.hpp>
void HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize) {
	KInfo *info = GetInfo();

	char buffer[1024];
	size_t remaining;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	void *heapAddr = Malloc(executableSize);
	
	for (size_t i = 0; i < executableSize; i += 1024) {
		remaining = executableSize - i;

		VMM::LoadVirtualSpace(procSpace);
		memcpy(buffer, executableBase + i, remaining > 1024 ? 1024 : remaining);

		VMM::LoadVirtualSpace(info->kernelVirtualSpace);
		memcpy(heapAddr + i, buffer, remaining > 1024 ? 1024 : remaining);
	}

	LoadELF(ELF_CORE_MODULE, heapAddr, executableSize);

	VMM::LoadVirtualSpace(procSpace);
}

void HandleSyscallProcFork(size_t TODO) {
}

void HandleSyscallProcReturn(size_t returnCode, uintptr_t stack) {
	static size_t pid = 1;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	PRINTK::PrintK("Returning: %d form 0x%x\r\n", returnCode, stack); 

	/* TMP FIX */
	size_t maxPID = info->kernelScheduler->GetMaxPID();

	if (pid + 1 > maxPID) while(true);

	info->kernelScheduler->SwitchToTask(++pid, 0);

	while(true);
}

void HandleSyscallProcExit(size_t exitCode, uintptr_t stack) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	PRINTK::PrintK("Exiting: %d form 0x%x\r\n", exitCode, stack); 
	
	while(true);
}

void HandleSyscallProcWait(size_t TODO) {
}

void HandleSyscallProcKill(size_t TODO) {
}

void HandleSyscallModuleRegister(size_t TODO) {
}

void HandleSyscallModuleUnregister(size_t TODO) {
}

#include <sys/file.hpp>
#include <mm/string.hpp>
void HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length) {
	KInfo *info = GetInfo();

	//size_t filenameLength = strlen(filename);
	//filenameLength = filenameLength > 512 ? 512 : filenameLength;

	char newFilename[256];
	strcpy(newFilename, filename);
	size_t newLength;
	void *newAddr;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	newAddr = FILE::Open(newFilename, &newLength);
	
	VMM::LoadVirtualSpace(procSpace);

	*address = newAddr;
	*length = newLength;
}

void HandleSyscallFileRead(size_t TODO) {
}

void HandleSyscallFileWrite(size_t TODO) {
}

void HandleSyscallFileClose(size_t TODO) {
}

void HandleSyscallKernOverride(size_t TODO) {
}
