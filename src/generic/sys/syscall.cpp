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
SyscallFunctionCallback *vector;
void InitSyscalls() {
	KInfo *info = GetInfo();
	vector = (SyscallFunctionCallback*)((uintptr_t)PMM::RequestPage() + info->HigherHalfMapping);
	if(vector == NULL) PANIC("Unable to allocate for the syscall vector");
	
	Memset(vector, 0, PAGE_SIZE);

	PRINTK::PrintK("Initializing system call API.\r\n");

	vector[SYSCALL_DEBUG_PRINTK] = (SyscallFunctionCallback)(void*)HandleSyscallDebugPrintK;

	vector[SYSCALL_MEMORY_GETINFO] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryGetinfo;
	vector[SYSCALL_MEMORY_VMALLOC] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryVmalloc;
	vector[SYSCALL_MEMORY_PALLOC] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryPalloc;
	vector[SYSCALL_MEMORY_VMFREE] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryVmfree;
	vector[SYSCALL_MEMORY_MMAP] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryMmap;
	vector[SYSCALL_MEMORY_UNMAP] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryUnmap;
	vector[SYSCALL_MEMORY_INOUT] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryInOut;

	vector[SYSCALL_PROC_EXEC] = (SyscallFunctionCallback)(void*)HandleSyscallProcExec;
	vector[SYSCALL_PROC_FORK] = (SyscallFunctionCallback)(void*)HandleSyscallProcFork;
	vector[SYSCALL_PROC_RETURN] = (SyscallFunctionCallback)(void*)HandleSyscallProcReturn;
	vector[SYSCALL_PROC_EXIT] = (SyscallFunctionCallback)(void*)HandleSyscallProcExit;
	vector[SYSCALL_PROC_WAIT] = (SyscallFunctionCallback)(void*)HandleSyscallProcWait;
	vector[SYSCALL_PROC_KILL] = (SyscallFunctionCallback)(void*)HandleSyscallProcKill;

	vector[SYSCALL_MODULE_REGISTER] = (SyscallFunctionCallback)(void*)HandleSyscallModuleRegister;
	vector[SYSCALL_MODULE_UNREGISTER] = (SyscallFunctionCallback)(void*)HandleSyscallModuleUnregister;
	vector[SYSCALL_MODULE_BUFFER_CREATE] = (SyscallFunctionCallback)(void*)HandleSyscallModuleBufferCreate;
	vector[SYSCALL_MODULE_BUFFER_MAP] = (SyscallFunctionCallback)(void*)HandleSyscallModuleBufferMap;
	vector[SYSCALL_MODULE_BUFFER_UNMAP] = (SyscallFunctionCallback)(void*)HandleSyscallModuleBufferUnmap;
	vector[SYSCALL_MODULE_BUFFER_DELETE] = (SyscallFunctionCallback)(void*)HandleSyscallModuleBufferDelete;
	vector[SYSCALL_MODULE_MESSAGE_HANDLER] = (SyscallFunctionCallback)(void*)HandleSyscallModuleMessageHandler;
	vector[SYSCALL_MODULE_MESSAGE_SEND] = (SyscallFunctionCallback)(void*)HandleSyscallModuleMessageSend;
	vector[SYSCALL_MODULE_SECTION_REGISTER] = (SyscallFunctionCallback)(void*)HandleSyscallModuleSectionRegister;
	vector[SYSCALL_MODULE_SECTION_GET] = (SyscallFunctionCallback)(void*)HandleSyscallModuleSectionGet;
	vector[SYSCALL_MODULE_SECTION_UNREGISTER] = (SyscallFunctionCallback)(void*)HandleSyscallModuleSectionUnregister;

	vector[SYSCALL_FILE_OPEN] = (SyscallFunctionCallback)(void*)HandleSyscallFileOpen;
	vector[SYSCALL_FILE_READ] = (SyscallFunctionCallback)(void*)HandleSyscallFileRead;
	vector[SYSCALL_FILE_WRITE] = (SyscallFunctionCallback)(void*)HandleSyscallFileWrite;
	vector[SYSCALL_FILE_CLOSE] = (SyscallFunctionCallback)(void*)HandleSyscallFileClose;

	vector[SYSCALL_KERN_OVERRIDE] = (SyscallFunctionCallback)(void*)HandleSyscallKernOverride;

	(void)info;

}

extern "C" size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6) {
	/* Check first if the syscall has been overridden. */
	size_t override = CheckOverride(syscallNumber);
	if(override != 0) return RunOverride(override);

	/* The syscall was not overridden, execute the normal kernel call */
	if(syscallNumber < 1 || syscallNumber > 30) return syscallNumber;

	return vector[syscallNumber](arg1, arg2, arg3, arg4, arg5, arg6);
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

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		uintptr_t paddr = (uintptr_t)PMM::RequestPage();
		if (paddr == 0) PANIC("Out of memory");

		Memset((void*)(paddr + info->HigherHalfMapping), 0, PAGE_SIZE);

		if (flags == 0) VMM::MapMemory(procSpace, (void*)paddr, (void*)vaddr);
		else VMM::MapMemory(procSpace, (void*)paddr, (void*)vaddr, flags);
	}

	return 0;
}

size_t HandleSyscallMemoryPalloc(uintptr_t *base, size_t length) {
	uintptr_t newBase = 0;

	size_t roundedLength = length / PAGE_SIZE + 1;
	if (length % PAGE_SIZE == 0) newBase = (uintptr_t)PMM::RequestPage();
	else newBase = (uintptr_t)PMM::RequestPages(roundedLength);

	*base = newBase;

	return 0;
}

size_t HandleSyscallMemoryVmfree(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1;

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

	return 0;
}

size_t HandleSyscallMemoryMmap(uintptr_t src, uintptr_t dest, size_t length, size_t flags) {
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

	return 0;
}

size_t HandleSyscallMemoryUnmap(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1; /* Make sure it is in valid memory */

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t end = base + length; base < end; base += PAGE_SIZE) {
		procSpace->UnmapMemory((void*)base);
	}

	return 0;
}

size_t HandleSyscallMemoryInOut(uintptr_t port, bool out, size_t outData, size_t *inData, uint8_t size) {
	size_t tmpInData = 0;
	
	if(!out && inData == NULL) return -1;

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
	
	if(!out) *inData = tmpInData;

	return 0;
}


size_t HandleSyscallProcExec(uintptr_t executableBase, size_t executableSize) {
	size_t heapSize = (executableSize + (PAGE_SIZE - executableSize % PAGE_SIZE));
	uint8_t *heapAddr = (uint8_t*)Malloc(heapSize);
	Memset(heapAddr, 0, heapSize);
	Memcpy(heapAddr, (void*)executableBase, executableSize);
	
	size_t pid = LoadExecutableFile((uint8_t*)heapAddr, executableSize);
	PRINTK::PrintK("New process is PID: 0x%x\r\n", pid);
	Free(heapAddr);

	/* TODO: fix, Buggy, do not use
	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, pid, 0), PROC::ExecutableUnitState::P_READY);
	*/

	return 0;
}

size_t HandleSyscallProcFork(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallProcReturn(size_t returnCode, uintptr_t stack) {
	KInfo *info = GetInfo();

	PRINTK::PrintK("Returning: %d form 0x%x\r\n", returnCode, stack); 

//	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, proc->ID, 0), PROC::ExecutableUnitState::P_WAITING);

	PROC::PrintSchedulerStatus(info->KernelScheduler);

#if defined(ARCH_x64)
	asm volatile ("sti");
#endif
	while(true) {  }

	return 0;
}

size_t HandleSyscallProcExit(size_t exitCode, uintptr_t stack) {
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

	PROC::UserProcess *proc = GetProcess();

	info->KernelModuleManager->RegisterModule(proc, vendorID, productID);

	return 0;
}

size_t HandleSyscallModuleUnregister() {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod != NULL) info->KernelModuleManager->UnregisterModule(mod->GetVendor(), mod->GetProduct());
	
	return 0;
}

size_t HandleSyscallModuleBufferCreate(size_t size, size_t type, uint32_t *id) {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) {
		return 0;
	}

	MODULE::Buffer *buf = info->KernelBufferManager->CreateBuffer(mod->GetVendor(), mod->GetProduct(), static_cast<MODULE::BufferType>(type), size);
	uint32_t tmpID = buf->ID;

	*id = tmpID;

	return 0;
}

size_t HandleSyscallModuleBufferMap(uintptr_t address, uint32_t id) {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace(proc);
	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return 0;

	int result = info->KernelBufferManager->MapBuffer(mod->GetVendor(), mod->GetProduct(), id, procSpace, address);

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
	PROC::UserProcess *proc = GetProcess();
	(void)proc;

	(void)entry;
	/* TODO: fix this
	proc->SetMessageThread(entry);
	*/

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
		Memcpy((void*)buffer, (void*)((uintptr_t)data + i), remaining > bufferSize ? bufferSize : remaining);

		VMM::LoadVirtualSpace(info->KernelVirtualSpace);
		VMM::LoadVirtualSpace(receiverProcSpace);

		Memcpy((void*)((uintptr_t)baseAddr + i), (void*)buffer, remaining > bufferSize ? bufferSize : remaining);
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
	Memcpy((void*)newSectionName, (void*)sectionName, sectionLength);

	PROC::UserProcess *proc = GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return -1;

	info->KernelSectionManager->RegisterSectionDriver(newSectionName, mod->GetVendor(), mod->GetProduct());

	return 0;
}

size_t HandleSyscallModuleSectionGet(const char *sectionName, uint32_t *vendorID, uint32_t *productID) {
	KInfo *info = GetInfo();

	char newSectionName[256];
	strcpy(newSectionName, sectionName);
	uint32_t newVendor, newProduct;

	info->KernelSectionManager->GetSectionDriver(newSectionName, &newVendor, &newProduct);
	
	*vendorID = newVendor;
	*productID = newProduct;

	return 0;
}

size_t HandleSyscallModuleSectionUnregister(const char *sectionName) {
	KInfo *info = GetInfo();

	size_t sectionLength = strlen(sectionName);
	sectionLength = sectionLength > 256 ? 256 : sectionLength;

	char newSectionName[256];
	Memcpy((void*)newSectionName, (void*)sectionName, sectionLength);

	PROC::UserProcess *proc = GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return 0;

	info->KernelSectionManager->UnregisterSectionDriver(newSectionName, mod->GetVendor(), mod->GetProduct());

	return 0;
}

size_t HandleSyscallFileOpen(char *filename, uintptr_t *address, size_t *length) {
	KInfo *info = GetInfo();

	size_t filenameLength = strlen(filename);
	filenameLength = filenameLength > 512 ? 512 : filenameLength;

	char newFilename[512] = {0};
	Memcpy((void*)newFilename, (void*)filename, filenameLength);

	*address = (uintptr_t)FILE::Open(newFilename, length) - info->HigherHalfMapping;

	return 0;
}

size_t HandleSyscallFileRead(char *filename, uintptr_t address, size_t length) {
	size_t filenameLength = strlen(filename);
	filenameLength = filenameLength > 512 ? 512 : filenameLength;

	char newFilename[512] = {0};
	Memcpy((void*)newFilename, (void*)filename, filenameLength);

	size_t fileLength;
	uintptr_t fileAddr;

	fileAddr = (uintptr_t)FILE::Open(newFilename, &fileLength);

	Memcpy((void*)address, (void*)fileAddr, length > fileLength ? fileLength : length);
	
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
