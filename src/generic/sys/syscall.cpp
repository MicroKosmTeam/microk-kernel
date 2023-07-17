#include <sys/syscall.hpp>
#include <stdint.h>
#include <stddef.h>
#include <cdefs.h>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>
#include <module/modulemanager.hpp>
#include <sys/loader.hpp>
#include <module/module.hpp>
#include <module/buffer.hpp>
#include <module/message.hpp>
#include <mm/string.hpp>
#include <sys/file.hpp>

// TMP measure: do something better with SMP
__attribute__((__aligned__((16)))) __attribute__((section(".syscall.stack"))) volatile char SyscallStack[128 * 1024];
__attribute__((section(".syscall.stack"))) __attribute__((__aligned__((16)))) extern "C" void *StartSyscallStack = &SyscallStack[128 * 1024 - 1];

void AddOverride(size_t syscallNumber);
size_t CheckOverride(size_t syscallNumber);
size_t RunOverride(size_t syscallNumber);

/* Kernel syscall handlers */
size_t HandleSyscallDebugPrintK(const char *string);

size_t HandleSyscallMemoryGetinfo(uintptr_t structbase);
size_t HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags);
size_t HandleSyscallMemoryVmfree(uintptr_t base, size_t length);
size_t HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags);
size_t HandleSyscallMemoryUnmap(uintptr_t base, size_t length);

size_t HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize);
size_t HandleSyscallProcFork(size_t TODO);
size_t HandleSyscallProcReturn(size_t returnCode, uintptr_t stack);
size_t HandleSyscallProcExit(size_t exitCode, uintptr_t stack);
size_t HandleSyscallProcWait(size_t TODO);
size_t HandleSyscallProcKill(size_t TODO);

size_t HandleSyscallModuleRegister(size_t vendorID, size_t productID);
size_t HandleSyscallModuleUnregister();
size_t HandleSyscallModuleBufferRegister(uintptr_t virtualBase, size_t size, size_t type);
size_t HandleSyscallModuleBufferUnregister(uint32_t id);
size_t HandleSyscallModuleMessageSend(uint32_t vendorID, uint32_t productID, uint32_t senderBufferID, size_t method, uint32_t receiverBufferID, size_t size);
size_t HandleSyscallModuleMessageReceive(uint32_t bufferID);
size_t HandleSyscallModuleBusRegister(const char *busName);
size_t HandleSyscallModuleBusGet(const char *busName, uint32_t *vendorID, uint32_t *productID);
size_t HandleSyscallModuleBusUnregister(const char *busName);

size_t HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length);
size_t HandleSyscallFileRead(size_t TODO);
size_t HandleSyscallFileWrite(size_t TODO);
size_t HandleSyscallFileClose(size_t TODO);

size_t HandleSyscallKernOverride(size_t TODO);

extern "C" size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6) {
/*	if (syscallNumber != SYSCALL_DEBUG_PRINTK && syscallNumber != SYSCALL_MEMORY_MMAP)
	PRINTK::PrintK("\r\n!!! SYSCALL !!!\r\n"
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

		case SYSCALL_MODULE_REGISTER: return HandleSyscallModuleRegister(arg1, arg2);
		case SYSCALL_MODULE_UNREGISTER: return HandleSyscallModuleUnregister();
		case SYSCALL_MODULE_BUFFER_REGISTER: return HandleSyscallModuleBufferRegister(arg1, arg2, arg3);
		case SYSCALL_MODULE_BUFFER_UNREGISTER: return HandleSyscallModuleBufferUnregister(arg1);
		case SYSCALL_MODULE_MESSAGE_SEND: return HandleSyscallModuleMessageSend(arg1, arg2, arg3, arg4, arg5, arg6);
		case SYSCALL_MODULE_MESSAGE_RECEIVE: return HandleSyscallModuleMessageReceive(arg1);
		case SYSCALL_MODULE_BUS_REGISTER: return HandleSyscallModuleBusRegister(arg1);
		case SYSCALL_MODULE_BUS_GET: return HandleSyscallModuleBusGet(arg1, arg2, arg3);
		case SYSCALL_MODULE_BUS_UNREGISTER: return HandleSyscallModuleBusUnregister(arg1);

		case SYSCALL_FILE_OPEN: return HandleSyscallFileOpen(arg1, arg2, arg3);
		case SYSCALL_FILE_READ: return HandleSyscallFileRead(0);
		case SYSCALL_FILE_WRITE: return HandleSyscallFileWrite(0);
		case SYSCALL_FILE_CLOSE: return HandleSyscallFileClose(0);

		case SYSCALL_KERN_OVERRIDE: return HandleSyscallKernOverride(0);

		default: return 0;
	}
}

void AddOverride(size_t syscallNumber) { return; }
size_t CheckOverride(size_t syscallNumber) { return 0; }
size_t RunOverride(size_t syscallNumber) { return 0; }


size_t HandleSyscallDebugPrintK(const char *string) {
	PRINTK::PrintK("%s", string);

	return 0;
}

size_t HandleSyscallMemoryGetinfo(uintptr_t structbase) {
	if (structbase <= 0x1000 || structbase >= 0x00007FFFFFFFFFFF)
		return; /* Make sure it is in valid memory */

	size_t *data = structbase;
	data[0] = PMM::GetFreeMem() + PMM::GetUsedMem();  /* Total */
	data[1] = PMM::GetFreeMem(); /* Free */
	data[2] = PMM::GetUsedMem(); /* Reserved (todo, get correct amount) */
	data[3] = 0; /* Buffers */

	return 0;
}

size_t HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags) {
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

		if (flags == 0) VMM::MapMemory(procSpace, paddr, vaddr);
		else VMM::MapMemory(procSpace, paddr, vaddr, flags);
	}

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallMemoryVmfree(uintptr_t base, size_t length) {
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

	return 0;
}

size_t HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags) {
	KInfo *info = GetInfo();
	
	if (src > info->higherHalfMapping) src -= info->higherHalfMapping;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	if (src % PAGE_SIZE) src -= src % PAGE_SIZE;
	if (dest % PAGE_SIZE) dest -= dest % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	uintptr_t end = src + length;
	for (; src < end; src += PAGE_SIZE, dest += PAGE_SIZE) {
		if (true) VMM::MapMemory(procSpace, src, dest);
		else VMM::MapMemory(procSpace, src, dest, flags);
	}

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallMemoryUnmap(uintptr_t base, size_t length) {
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

	return 0;
}


size_t HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize) {
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

	size_t pid = LoadExecutableFile(heapAddr, executableSize);
	info->kernelScheduler->SetProcessState(pid, PROC::P_READY);

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallProcFork(size_t TODO) {
	return 0;
}

size_t HandleSyscallProcReturn(size_t returnCode, uintptr_t stack) {
	static size_t pid = 1;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	PRINTK::PrintK("Returning: %d form 0x%x\r\n", returnCode, stack); 

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	info->kernelScheduler->SetProcessState(proc->GetPID(), PROC::P_WAITING);

	while(true) {
		info->kernelScheduler->RecalculateScheduler();
	}

	return 0;
}

size_t HandleSyscallProcExit(size_t exitCode, uintptr_t stack) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	PRINTK::PrintK("Exiting: %d form 0x%x\r\n", exitCode, stack); 
	
	while(true);

	return 0;
}

size_t HandleSyscallProcWait(size_t TODO) {
	return 0;
}

size_t HandleSyscallProcKill(size_t TODO) {
	return 0;
}

size_t HandleSyscallModuleRegister(size_t vendorID, size_t productID) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	info->KernelModuleManager->RegisterModule(proc, vendorID, productID);
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleUnregister() {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod != NULL) info->KernelModuleManager->UnregisterModule(mod->GetVendor(), mod->GetProduct());
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleBufferRegister(uintptr_t virtualBase, size_t size, size_t type) {
	KInfo *info = GetInfo();
	uint32_t tmpID = 0;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod == NULL) {
		VMM::LoadVirtualSpace(procSpace);

		return tmpID;
	}

	mod->RegisterBuffer(virtualBase, type, size, &tmpID);

	VMM::LoadVirtualSpace(procSpace);

	return tmpID;
}

size_t HandleSyscallModuleBufferUnregister(uint32_t id) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod == NULL) return -1;

	mod->UnregisterBuffer(id);
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleMessageSend(uint32_t vendorID, uint32_t productID, uint32_t senderBufferID, size_t method, uint32_t receiverBufferID, size_t size) {
	(void)method; /* Not yet used */

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod == NULL) return -1;

	MODULE::Buffer *buf = mod->GetBuffer(senderBufferID);
	if (buf == 0) return -1;
	
	if(MODULE::LockBuffer(buf) != 0) return -1;

	MODULE::Message *msg = buf->Address;
	MODULE::ComposeMessage(msg, mod->GetVendor(), mod->GetProduct(), size);
	MODULE::SendMailboxMessage(vendorID, productID, receiverBufferID, msg);

	if(MODULE::UnlockBuffer(buf) != 0) return -1;
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleMessageReceive(uint32_t bufferID) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);

	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod == NULL) return -1;


	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleBusRegister(const char *busName) {
	KInfo *info = GetInfo();

	size_t busLength = strlen(busName);
	busLength = busLength > 256 ? 256 : busLength;

	char newBusName[256] = { 0 };
	memcpy(newBusName, busName, busLength);

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod == NULL) return;

	info->KernelBusManager->RegisterBusDriver(newBusName, mod->GetVendor(), mod->GetProduct());
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleBusGet(const char *busName, uint32_t *vendorID, uint32_t *productID) {
	KInfo *info = GetInfo();

	size_t busLength = strlen(busName);
	busLength = busLength > 256 ? 256 : busLength;

	char newBusName[256];
	memcpy(newBusName, busName, busLength);
	uint32_t newVendor, newProduct;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	info->KernelBusManager->GetBusDriver(newBusName, &newVendor, &newProduct);
	
	VMM::LoadVirtualSpace(procSpace);

	*vendorID = newVendor;
	*productID = newProduct;

	return 0;
}

size_t HandleSyscallModuleBusUnregister(const char *busName) {
	KInfo *info = GetInfo();

	size_t busLength = strlen(busName);
	busLength = busLength > 256 ? 256 : busLength;

	char newBusName[256];
	memcpy(newBusName, busName, busLength);

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	PROC::Process *proc = info->kernelScheduler->GetRunningProcess();
	VMM::VirtualSpace *procSpace = proc->GetVirtualMemorySpace();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->GetPID());
	if (mod == NULL) return;

	info->KernelBusManager->UnregisterBusDriver(newBusName, mod->GetVendor(), mod->GetProduct());
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length) {
	KInfo *info = GetInfo();

	size_t filenameLength = strlen(filename);
	filenameLength = filenameLength > 512 ? 512 : filenameLength;

	char newFilename[512];
	memcpy(newFilename, filename, filenameLength);
	size_t newLength;
	size_t *newAddr;

	VMM::LoadVirtualSpace(info->kernelVirtualSpace);
	
	VMM::VirtualSpace *procSpace = info->kernelScheduler->GetRunningProcess()->GetVirtualMemorySpace();

	newAddr = FILE::Open(newFilename, &newLength);
	
	VMM::LoadVirtualSpace(procSpace);

	*address = newAddr;
	*length = newLength;

	return 0;
}

size_t HandleSyscallFileRead(size_t TODO) {
	return 0;
}

size_t HandleSyscallFileWrite(size_t TODO) {
	return 0;
}

size_t HandleSyscallFileClose(size_t TODO) {
	return 0;
}

size_t HandleSyscallKernOverride(size_t TODO) {
	return 0;
}
