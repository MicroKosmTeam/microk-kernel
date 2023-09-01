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
#include <mm/memory.hpp>
#include <sys/file.hpp>
#include <sys/mutex.hpp>

#if defined(ARCH_x64)
#include <arch/x64/io/io.hpp>
#endif

static inline PROC::UserProcess *GetProcess() {
	KInfo *info = GetInfo();

	LockMutex(&info->KernelScheduler->SchedulerLock);
	PROC::UserProcess *proc = (PROC::UserProcess*)info->KernelScheduler->CurrentThread->Thread->Parent;
	UnlockMutex(&info->KernelScheduler->SchedulerLock);

	return proc;
}

static inline VMM::VirtualSpace *GetVirtualSpace(PROC::UserProcess *proc) {
	KInfo *info = GetInfo();

	LockMutex(&info->KernelScheduler->SchedulerLock);
	VMM::VirtualSpace *procSpace = proc->VirtualMemorySpace;
	UnlockMutex(&info->KernelScheduler->SchedulerLock);

	return procSpace;
}

// TMP measure: do something better with SMP
__attribute__((__aligned__((16)))) __attribute__((section(".syscall.stack"))) volatile char SyscallStack[128 * 1024];
__attribute__((section(".syscall.stack"))) __attribute__((__aligned__((16)))) volatile void *StartSyscallStack = &SyscallStack[128 * 1024 - 1];

void AddOverride(size_t syscallNumber);
size_t CheckOverride(size_t syscallNumber);
size_t RunOverride(size_t syscallNumber);

/* Kernel syscall handlers */
size_t HandleSyscallDebugPrintK(const char *string);

size_t HandleSyscallMemoryGetinfo(uintptr_t structbase);
size_t HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags);
size_t HandleSyscallMemoryPalloc(uintptr_t *base, size_t length);
size_t HandleSyscallMemoryVmfree(uintptr_t base, size_t length);
size_t HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags);
size_t HandleSyscallMemoryUnmap(uintptr_t base, size_t length);
size_t HandleSyscallMemoryInOut(uintptr_t port, bool out, size_t outData, size_t *inData, uint8_t size);

size_t HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize);
size_t HandleSyscallProcFork(size_t TODO);
size_t HandleSyscallProcReturn(size_t returnCode, uintptr_t stack);
size_t HandleSyscallProcExit(size_t exitCode, uintptr_t stack);
size_t HandleSyscallProcWait(size_t TODO);
size_t HandleSyscallProcKill(size_t TODO);

size_t HandleSyscallModuleRegister(size_t vendorID, size_t productID);
size_t HandleSyscallModuleUnregister();
size_t HandleSyscallModuleBufferCreate(size_t size, size_t type, uint32_t *id);
size_t HandleSyscallModuleBufferMap(uintptr_t address, uint32_t id);
size_t HandleSyscallModuleBufferUnmap(uintptr_t address, uint32_t id);
size_t HandleSyscallModuleBufferDelete(uint32_t id);
size_t HandleSyscallModuleMessageHandler(uintptr_t entry);
size_t HandleSyscallModuleMessageSend(uint32_t vendorID, uint32_t productID, void *data, size_t size);
size_t HandleSyscallModuleSectionRegister(const char *sectionName);
size_t HandleSyscallModuleSectionGet(const char *sectionName, uint32_t *vendorID, uint32_t *productID);
size_t HandleSyscallModuleSectionUnregister(const char *sectionName);

size_t HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length);
size_t HandleSyscallFileRead(char *filename, uintptr_t address, size_t length);
size_t HandleSyscallFileWrite(size_t TODO);
size_t HandleSyscallFileClose(size_t TODO);

size_t HandleSyscallKernOverride(size_t TODO);

extern "C" size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6) {
	/* Check first if the syscall has been overridden. */
	size_t override = CheckOverride(syscallNumber);
	if(override != 0) return RunOverride(override);

	/* The syscall was not overridden, execute the normal kernel call */
	switch(syscallNumber) {
		case SYSCALL_DEBUG_PRINTK: return HandleSyscallDebugPrintK((const char*)arg1);

		case SYSCALL_MEMORY_GETINFO: return HandleSyscallMemoryGetinfo((uintptr_t)arg1);
		case SYSCALL_MEMORY_VMALLOC: return HandleSyscallMemoryVmalloc((uintptr_t)arg1, arg2, arg3);
		case SYSCALL_MEMORY_PALLOC: return HandleSyscallMemoryPalloc((uintptr_t*)arg1, arg2);
		case SYSCALL_MEMORY_VMFREE: return HandleSyscallMemoryVmfree((uintptr_t)arg1, arg2);
		case SYSCALL_MEMORY_MMAP: return HandleSyscallMemoryMmap((uintptr_t)arg1, (uintptr_t)arg2, arg3, arg4);
		case SYSCALL_MEMORY_UNMAP: return HandleSyscallMemoryUnmap((uintptr_t)arg1, arg2);
		case SYSCALL_MEMORY_INOUT: return HandleSyscallMemoryInOut((uintptr_t)arg1, (bool)arg2, arg3, (size_t*)arg4, (uint8_t)arg5);

		case SYSCALL_PROC_EXEC: return HandleSyscallProcExec((uintptr_t)arg1, arg2);
		case SYSCALL_PROC_FORK: return HandleSyscallProcFork(arg6);
		case SYSCALL_PROC_RETURN: return HandleSyscallProcReturn(arg1, (uintptr_t)arg2);
		case SYSCALL_PROC_EXIT: return HandleSyscallProcExit(arg1, (uintptr_t)arg2);
		case SYSCALL_PROC_WAIT: return HandleSyscallProcWait(arg6);
		case SYSCALL_PROC_KILL: return HandleSyscallProcKill(arg6);

		case SYSCALL_MODULE_REGISTER: return HandleSyscallModuleRegister(arg1, arg2);
		case SYSCALL_MODULE_UNREGISTER: return HandleSyscallModuleUnregister();
		case SYSCALL_MODULE_BUFFER_CREATE: return HandleSyscallModuleBufferCreate(arg1, arg2, (uint32_t*)arg3);
		case SYSCALL_MODULE_BUFFER_MAP: return HandleSyscallModuleBufferMap((uintptr_t)arg1, (uint32_t)arg2);
		case SYSCALL_MODULE_BUFFER_UNMAP: return HandleSyscallModuleBufferUnmap((uintptr_t)arg1, (uint32_t)arg2);
		case SYSCALL_MODULE_BUFFER_DELETE: return HandleSyscallModuleBufferDelete((uint32_t)arg1);
		case SYSCALL_MODULE_MESSAGE_HANDLER: return HandleSyscallModuleMessageHandler((uintptr_t)arg1);
		case SYSCALL_MODULE_MESSAGE_SEND: return HandleSyscallModuleMessageSend((uint32_t)arg1, (uint32_t)arg2, (void*)arg3, arg4);
		case SYSCALL_MODULE_SECTION_REGISTER: return HandleSyscallModuleSectionRegister((const char*)arg1);
		case SYSCALL_MODULE_SECTION_GET: return HandleSyscallModuleSectionGet((const char*)arg1, (uint32_t*)arg2, (uint32_t*)arg3);
		case SYSCALL_MODULE_SECTION_UNREGISTER: return HandleSyscallModuleSectionUnregister((const char*)arg1);

		case SYSCALL_FILE_OPEN: return HandleSyscallFileOpen((char*)arg1, (uintptr_t*)arg2, (size_t*)arg3);
		case SYSCALL_FILE_READ: return HandleSyscallFileRead((char*)arg1, (uintptr_t)arg2, arg3);
		case SYSCALL_FILE_WRITE: return HandleSyscallFileWrite(arg6);
		case SYSCALL_FILE_CLOSE: return HandleSyscallFileClose(arg6);

		case SYSCALL_KERN_OVERRIDE: return HandleSyscallKernOverride(arg6);

		default: return 0;
	}
}

void AddOverride(size_t syscallNumber) {
	(void)syscallNumber;
	return;
}

size_t CheckOverride(size_t syscallNumber) {
	(void)syscallNumber;
	return 0;
}

size_t RunOverride(size_t syscallNumber) {
	(void)syscallNumber;
	return 0;
}


size_t HandleSyscallDebugPrintK(const char *string) {
	PRINTK::PrintK("%s", string);

	return 0;
}

size_t HandleSyscallMemoryGetinfo(uintptr_t structbase) {
	if (structbase <= 0x1000 || structbase >= 0x00007FFFFFFFFFFF)
		return -1; /* Make sure it is in valid memory */

	size_t *data = (size_t*)structbase;
	data[0] = PMM::GetFreeMem() + PMM::GetUsedMem();  /* Total */
	data[1] = PMM::GetFreeMem(); /* Free */
	data[2] = PMM::GetUsedMem(); /* Reserved (todo, get correct amount) */
	data[3] = 0; /* Buffers */

	return 0;
}

size_t HandleSyscallMemoryVmalloc(uintptr_t base, size_t length, size_t flags) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		uintptr_t paddr = (uintptr_t)PMM::RequestPage();
		if (paddr == 0) PANIC("Out of memory");

		if (flags == 0) VMM::MapMemory(procSpace, (void*)paddr, (void*)vaddr);
		else VMM::MapMemory(procSpace, (void*)paddr, (void*)vaddr, flags);
	}

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallMemoryPalloc(uintptr_t *base, size_t length) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	uintptr_t newBase = 0;

	size_t roundedLength = length / PAGE_SIZE + 1;
	if (length % PAGE_SIZE == 0) newBase = (uintptr_t)PMM::RequestPage();
	else newBase = (uintptr_t)PMM::RequestPages(roundedLength);

	VMM::LoadVirtualSpace(procSpace);

	*base = newBase;

	return 0;
}

size_t HandleSyscallMemoryVmfree(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		uintptr_t paddr = (uintptr_t)procSpace->GetPhysicalAddress((void*)vaddr);
		procSpace->UnmapMemory((void*)vaddr);
		if (paddr == 0) return -1;

		PMM::FreePage((void*)paddr);
	}

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags) {
	KInfo *info = GetInfo();
	
	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	if (src % PAGE_SIZE) src -= src % PAGE_SIZE;
	if (dest % PAGE_SIZE) dest -= dest % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	uintptr_t end = src + length;
	for (; src < end; src += PAGE_SIZE, dest += PAGE_SIZE) {
		if (flags == 0) VMM::MapMemory(procSpace, (void*)src, (void*)dest);
		else VMM::MapMemory(procSpace, (void*)src, (void*)dest, flags);
	}

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallMemoryUnmap(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1; /* Make sure it is in valid memory */

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t end = base + length; base < end; base += PAGE_SIZE) {
		procSpace->UnmapMemory((void*)base);
	}

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallMemoryInOut(uintptr_t port, bool out, size_t outData, size_t *inData, uint8_t size) {
	size_t tmpInData = 0;
	
	if(!out && inData == NULL) return -1;

	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

#if defined(ARCH_x64)
	using namespace x86_64;

	switch(size) {
		case 8:
			if(out) OutB(port, outData);
			else tmpInData = InB(port);
			break;
		case 16:
			if(out) OutW(port, outData);
			else tmpInData = InW(port);
			break;
		case 32:
			if(out) OutD(port, outData);
			else tmpInData = InD(port);
			break;
		case 64:
			if(out) {
				OutD(port, outData && 0xFFFFFFFF);
				OutD(port + 4, outData >> 32);
			} else {
				tmpInData = (size_t)InD(port);
				tmpInData |= (size_t)InD(port + 4) >> 32;
			}
			break;
		default:
			if(!out) tmpInData = -1;
			break;
	}
#else
	(void)port;
	(void)out;
	(void)outData;
	(void)size;
#endif
	
	VMM::LoadVirtualSpace(procSpace);

	if(!out) *inData = tmpInData;

	return 0;
}


size_t HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	uint8_t buffer[PAGE_SIZE];
	size_t remaining = 0;

	size_t heapSize = (executableSize / PAGE_SIZE + 1) * PAGE_SIZE;
	uint8_t *heapAddr = (uint8_t*)Malloc(heapSize);
	memset(heapAddr, 0, heapSize);
	
	for (size_t i = 0; i < executableSize; i += PAGE_SIZE) {
		remaining = executableSize - i;

		VMM::LoadVirtualSpace(procSpace);
		memcpy((void*)buffer, (void*)(executableBase + i), remaining > PAGE_SIZE ? PAGE_SIZE : remaining);

		VMM::LoadVirtualSpace(info->KernelVirtualSpace);
		memcpy((void*)((uintptr_t)heapAddr + i), (void*)buffer, remaining > PAGE_SIZE ? PAGE_SIZE : remaining);
	}

	size_t pid = LoadExecutableFile((uint8_t*)heapAddr, executableSize);
	PRINTK::PrintK("New process is PID: 0x%x\r\n", pid);
	Free(heapAddr);

	/* TODO: fix, Buggy, do not use
	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, pid, 0), PROC::ExecutableUnitState::P_READY);
	*/

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallProcFork(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallProcReturn(size_t returnCode, uintptr_t stack) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PRINTK::PrintK("Returning: %d form 0x%x\r\n", returnCode, stack); 

	PROC::UserProcess *proc = GetProcess();

	(void)proc;
//	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, proc->ID, 0), PROC::ExecutableUnitState::P_WAITING);

	PROC::PrintSchedulerStatus(info->KernelScheduler);
	
	while(true);

	return 0;
}

size_t HandleSyscallProcExit(size_t exitCode, uintptr_t stack) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	PRINTK::PrintK("Exiting: %d form 0x%x\r\n", exitCode, stack); 
	
	while(true);

	return 0;
}

size_t HandleSyscallProcWait(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallProcKill(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallModuleRegister(size_t vendorID, size_t productID) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	info->KernelModuleManager->RegisterModule(proc, vendorID, productID);
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleUnregister() {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod != NULL) info->KernelModuleManager->UnregisterModule(mod->GetVendor(), mod->GetProduct());
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleBufferCreate(size_t size, size_t type, uint32_t *id) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) {
		VMM::LoadVirtualSpace(procSpace);

		return 0;
	}

	MODULE::Buffer *buf = info->KernelBufferManager->CreateBuffer(mod->GetVendor(), mod->GetProduct(), static_cast<MODULE::BufferType>(type), size);
	uint32_t tmpID = buf->ID;

	VMM::LoadVirtualSpace(procSpace);

	*id = tmpID;

	return 0;
}

size_t HandleSyscallModuleBufferMap(uintptr_t address, uint32_t id) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) {
		VMM::LoadVirtualSpace(procSpace);

		return 0;
	}

	int result = info->KernelBufferManager->MapBuffer(mod->GetVendor(), mod->GetProduct(), id, procSpace, address);

	VMM::LoadVirtualSpace(procSpace);

	return result;
}

size_t HandleSyscallModuleBufferUnmap(uintptr_t address, uint32_t id) {
	(void)address;
	(void)id;
	return 0;
}

size_t HandleSyscallModuleBufferDelete(uint32_t id) {
	(void)id;
	return 0;
}

size_t HandleSyscallModuleMessageHandler(uintptr_t entry) {
	KInfo *info = GetInfo();

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	(void)entry;
	/* TODO: fix this
	proc->SetMessageThread(entry);
	*/

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleMessageSend(uint32_t vendorID, uint32_t productID, void *data, size_t size) {
	if(size == 0) return -1;
	
	KInfo *info = GetInfo();
	const size_t bufferSize = PAGE_SIZE;
	char buffer[bufferSize];

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	MODULE::Module *sendMod = info->KernelModuleManager->GetModule(proc->ID);
	MODULE::Module *receiverMod = info->KernelModuleManager->GetModule(vendorID, productID);
	if (sendMod == NULL || receiverMod == NULL) {
		VMM::LoadVirtualSpace(procSpace);
		return -1;
	}

	PROC::UserProcess *receiverProc = (PROC::UserProcess*)receiverMod->GetProcess();
	VMM::VirtualSpace *receiverProcSpace = receiverProc->VirtualMemorySpace;

	size_t remaining = 0;
	void *baseAddr = (void*)0x700000000000;
	
	for (size_t i = 0; i < size; i += bufferSize) {
		remaining = size - i;

		VMM::LoadVirtualSpace(info->KernelVirtualSpace);
		void *paddr = PMM::RequestPage();
		if (paddr == NULL) PANIC("Out of memory");

		VMM::MapMemory(receiverProcSpace, paddr, (void*)((uintptr_t)baseAddr + i));

		VMM::LoadVirtualSpace(procSpace);
		memcpy((void*)buffer, (void*)((uintptr_t)data + i), remaining > bufferSize ? bufferSize : remaining);

		VMM::LoadVirtualSpace(info->KernelVirtualSpace);
		VMM::LoadVirtualSpace(receiverProcSpace);

		memcpy((void*)((uintptr_t)baseAddr + i), (void*)buffer, remaining > bufferSize ? bufferSize : remaining);
	}

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);

	void *paddr = receiverProcSpace->GetPhysicalAddress(baseAddr);

	MODULE::ComposeMessage((MODULE::Message*)paddr, sendMod->GetVendor(), sendMod->GetProduct(), size);

	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, receiverProc->ID, 0), PROC::ExecutableUnitState::P_MESSAGE);

	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleSectionRegister(const char *sectionName) {
	KInfo *info = GetInfo();

	size_t sectionLength = strlen(sectionName);
	sectionLength = sectionLength > 256 ? 256 : sectionLength;

	char newSectionName[256] = { 0 };
	memcpy((void*)newSectionName, (void*)sectionName, sectionLength);

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) { PRINTK::PrintK("NULLMODULE\r\n"); while(true); }

	info->KernelSectionManager->RegisterSectionDriver(newSectionName, mod->GetVendor(), mod->GetProduct());
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallModuleSectionGet(const char *sectionName, uint32_t *vendorID, uint32_t *productID) {
	KInfo *info = GetInfo();

	char newSectionName[256];
	strcpy(newSectionName, sectionName);
	uint32_t newVendor, newProduct;

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	info->KernelSectionManager->GetSectionDriver(newSectionName, &newVendor, &newProduct);
	
	VMM::LoadVirtualSpace(procSpace);

	*vendorID = newVendor;
	*productID = newProduct;

	return 0;
}

size_t HandleSyscallModuleSectionUnregister(const char *sectionName) {
	KInfo *info = GetInfo();

	size_t sectionLength = strlen(sectionName);
	sectionLength = sectionLength > 256 ? 256 : sectionLength;

	char newSectionName[256];
	memcpy((void*)newSectionName, (void*)sectionName, sectionLength);

	VMM::LoadVirtualSpace(info->KernelVirtualSpace);
	
	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return 0;

	info->KernelSectionManager->UnregisterSectionDriver(newSectionName, mod->GetVendor(), mod->GetProduct());
	
	VMM::LoadVirtualSpace(procSpace);

	return 0;
}

size_t HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length) {
	size_t filenameLength = strlen(filename);
	filenameLength = filenameLength > 512 ? 512 : filenameLength;

	char newFilename[512] = {0};
	memcpy((void*)newFilename, (void*)filename, filenameLength);

	*address = (uintptr_t)FILE::Open(newFilename, length);

	return 0;
}

size_t HandleSyscallFileRead(char *filename, uintptr_t address, size_t length) {
	size_t filenameLength = strlen(filename);
	filenameLength = filenameLength > 512 ? 512 : filenameLength;

	char newFilename[512] = {0};
	memcpy((void*)newFilename, (void*)filename, filenameLength);

	size_t fileLength;
	uintptr_t fileAddr;

	fileAddr = (uintptr_t)FILE::Open(newFilename, &fileLength);

	memcpy((void*)address, (void*)fileAddr, length > fileLength ? fileLength : length);
	
	return 0;
}

size_t HandleSyscallFileWrite(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallFileClose(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallKernOverride(size_t TODO) {
	(void)TODO;
	return 0;
}
