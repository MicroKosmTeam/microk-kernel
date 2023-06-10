#include <sys/syscall.hpp>
#include <stdarg.h>
#include <stdint.h>
#include <cdefs.h>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>

// TMP measure: do something better with SMP
__attribute__((__aligned__((16)))) __attribute__((section(".syscall.stack"))) char SyscallStack[128 * 1024];
__attribute__((section(".syscall.stack"))) __attribute__((__aligned__((16)))) extern "C" void *StartSyscallStack = &SyscallStack[128 * 1024 - 1];

void HandleSyscallDebugPrintK(const char *string);

void HandleSyscallMemoryGetinfo(uintptr_t structbase);
void HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags);
void HandleSyscallMemoryVmfree(uintptr_t base, size_t length);
void HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags);
void HandleSyscallMemoryUnmap(uintptr_t base, size_t length);

void HandleSyscallProcExec(size_t TODO);
void HandleSyscallProcFork(size_t TODO);
void HandleSyscallProcReturn(size_t returnCode, uintptr_t stack);
void HandleSyscallProcExit(size_t exitCode, uintptr_t stack);

extern "C" void HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5) {
	switch(syscallNumber) {
		case SYSCALL_DEBUG_PRINTK: return HandleSyscallDebugPrintK(arg1);
		case SYSCALL_MEMORY_GETINFO: return HandleSyscallMemoryGetinfo(arg1);
		case SYSCALL_MEMORY_VMALLOC: return HandleSyscallMemoryVmalloc(arg1, arg2, arg3);
		case SYSCALL_MEMORY_VMFREE: return HandleSyscallMemoryVmfree(arg1, arg2);
		case SYSCALL_MEMORY_MMAP: return HandleSyscallMemoryMmap(arg1, arg2, arg3, arg4);
		case SYSCALL_MEMORY_UNMAP: return HandleSyscallMemoryUnmap(arg1, arg2);
		case SYSCALL_PROC_EXEC: return HandleSyscallProcExec(arg1);
		case SYSCALL_PROC_FORK: return HandleSyscallProcFork(arg1);
		case SYSCALL_PROC_RETURN: return HandleSyscallProcReturn(arg1, arg2);
		case SYSCALL_PROC_EXIT: return HandleSyscallProcExit(arg1, arg2);
	}
}

void HandleSyscallDebugPrintK(const char *string) {
	PRINTK::PrintK(string);
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
		return; /* Make sure it is in valid memory */

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		void *paddr = PMM::RequestPage();
		if (paddr == NULL) PANIC("Out of memory");

		procSpace->MapMemory(paddr, vaddr, flags);
	}

	VMM::LoadVirtualSpace(procSpace);
}

void HandleSyscallMemoryVmfree(uintptr_t base, size_t length) {
	/* Process: check if all arguments are valid, just clear those pages and free in the bitmap*/
}

void HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags) {
	if (src <= 0x1000 || dest <= 0x1000 || src + length >= 0x00007FFFFFFFF000 || dest + length >= 0x00007FFFFFFFF000)
		return; /* Make sure it is in valid memory */

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (src % PAGE_SIZE) src -= src % PAGE_SIZE;
	if (dest % PAGE_SIZE) dest -= dest % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t end = src + dest; src < end; src += PAGE_SIZE, dest += PAGE_SIZE) {
		procSpace->MapMemory(src, dest, flags);
	}

	VMM::LoadVirtualSpace(procSpace);
}

void HandleSyscallMemoryUnmap(uintptr_t base, size_t length) {
	/* Process: check if all arguments are valid, just clear those pages*/
}

void HandleSyscallProcExec(size_t TODO) {
	// TODO
}

void HandleSyscallProcFork(size_t TODO) {
	// TODO
}

void HandleSyscallProcReturn(size_t returnCode, uintptr_t stack) {
	PRINTK::PrintK("Returning: %d form 0x%x\r\n", returnCode, stack); 

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PRINTK::PrintK("Back to the kernel.\r\n");

	while(true);
}

void HandleSyscallProcExit(size_t exitCode, uintptr_t stack) {
	PRINTK::PrintK("Exiting: %d form 0x%x\r\n", exitCode, stack); 
}
