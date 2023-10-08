#include <cdefs.h>
#include <stdint.h>
#include <stddef.h>
#include <mm/pmm.hpp>
#include <sys/io.hpp>
#include <sys/file.hpp>
#include <sys/user.hpp>
#include <mm/string.hpp>
#include <mm/memory.hpp>
#include <sys/mutex.hpp>
#include <sys/panic.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/loader.hpp>
#include <sys/syscall.hpp>
#include <proc/helpers.hpp>
#include <module/module.hpp>
#include <module/buffer.hpp>
#include <module/message.hpp>
#include <module/modulemanager.hpp>

#if defined(ARCH_x64)
#include <arch/x64/io/io.hpp>
#endif

/* Kernel syscall handlers */
size_t HandleSyscallDebugPrintK(const char *string);

size_t HandleSyscallMemoryVMAlloc(uintptr_t base, size_t length, size_t flags);
size_t HandleSyscallMemoryPMAlloc(uintptr_t *base, size_t length, size_t flags);
size_t HandleSyscallMemoryVMFree(uintptr_t base, size_t length);
size_t HandleSyscallMemoryMMap(uintptr_t src, uintptr_t dest, size_t length, size_t flags);
size_t HandleSyscallMemoryMProtect(uintptr_t base, size_t length, size_t flags);
size_t HandleSyscallMemoryUnMap(uintptr_t base, size_t length);
size_t HandleSyscallMemoryInOut(const_userptr_t iorequests, size_t count);

size_t HandleSyscallProcExec(userptr_t executableBase, size_t executableSize);
size_t HandleSyscallProcFork(size_t TODO);
size_t HandleSyscallProcReturn(size_t returnCode);
size_t HandleSyscallProcExit(size_t exitCode);
size_t HandleSyscallProcWait(size_t TODO);
size_t HandleSyscallProcSendSig(size_t TODO);

size_t HandleSyscallModuleRegister(size_t vendorID, size_t productID);
size_t HandleSyscallModuleUnregister();
size_t HandleSyscallModuleBufferCreate(size_t size, size_t type, uint32_t *id);
size_t HandleSyscallModuleBufferMap(uintptr_t address, uint32_t id);
size_t HandleSyscallModuleBufferUnMap(uintptr_t address, uint32_t id);
size_t HandleSyscallModuleBufferDelete(uint32_t id);
size_t HandleSyscallModuleMessageHandler(uintptr_t entry);
size_t HandleSyscallModuleMessageSend(uint32_t vendorID, uint32_t productID, void *data, size_t size);
size_t HandleSyscallModuleSectionRegister(const char *sectionName);
size_t HandleSyscallModuleSectionGet(const char *sectionName, uint32_t *vendorID, uint32_t *productID);
size_t HandleSyscallModuleSectionUnregister(const char *sectionName);

__attribute__((aligned(PAGE_SIZE))) __attribute__((section(".syscall")))
SyscallFunctionCallback SyscallVector[SYSCALL_VECTOR_END];

void InitSyscalls() {
	Memset(SyscallVector, 0, SYSCALL_VECTOR_END * sizeof(SyscallFunctionCallback));

	PRINTK::PrintK("Initializing system call API.\r\n");

	SyscallVector[SYSCALL_DEBUG_PRINTK] = (SyscallFunctionCallback)(void*)HandleSyscallDebugPrintK;

	SyscallVector[SYSCALL_MEMORY_VMALLOC] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryVMAlloc;
	SyscallVector[SYSCALL_MEMORY_PMALLOC] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryPMAlloc;
	SyscallVector[SYSCALL_MEMORY_VMFREE] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryVMFree;
	SyscallVector[SYSCALL_MEMORY_MMAP] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryMMap;
	SyscallVector[SYSCALL_MEMORY_UNMAP] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryUnMap;
	SyscallVector[SYSCALL_MEMORY_INOUT] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryInOut;

	SyscallVector[SYSCALL_PROC_EXEC] = (SyscallFunctionCallback)(void*)HandleSyscallProcExec;
	SyscallVector[SYSCALL_PROC_RETURN] = (SyscallFunctionCallback)(void*)HandleSyscallProcReturn;
}

extern "C" size_t HandleSyscall(size_t syscallNumber, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5, size_t arg6) {
	if(syscallNumber <= SYSCALL_VECTOR_START ||
	   syscallNumber >= SYSCALL_VECTOR_END ||
	   SyscallVector[syscallNumber] == NULL) return -ENOTPRESENT;

	return SyscallVector[syscallNumber](arg1, arg2, arg3, arg4, arg5, arg6);
}

size_t HandleSyscallDebugPrintK(const char *string) {
	PRINTK::PrintK("%s", string);

	return 0;
}

size_t HandleSyscallMemoryVMAlloc(uintptr_t base, size_t length, size_t flags) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1;

	KInfo *info = GetInfo();

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::ProcessBase*)proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		uintptr_t paddr = (uintptr_t)PMM::RequestPage();
		if (paddr == 0) /* TODO: do something better than calling */ MEM::InvokeOOM();

		Memset((void*)(paddr + info->HigherHalfMapping), 0, PAGE_SIZE);

		if (flags == 0) VMM::MapMemory(procSpace, (void*)paddr, (void*)vaddr);
		else VMM::MapMemory(procSpace, (void*)paddr, (void*)vaddr, flags);
	}

	return 0;
}

size_t HandleSyscallMemoryPMAlloc(uintptr_t *base, size_t length, size_t flags) {
	(void)flags;
	uintptr_t newBase = 0;

	size_t roundedLength = length / PAGE_SIZE + 1;
	if (length % PAGE_SIZE == 0) newBase = (uintptr_t)PMM::RequestPage();
	else newBase = (uintptr_t)PMM::RequestPages(roundedLength);

	*base = newBase;

	return 0;
}

size_t HandleSyscallMemoryVMFree(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1;

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::ProcessBase*)proc);

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

size_t HandleSyscallMemoryMMap(uintptr_t src, uintptr_t dest, size_t length, size_t flags) {
	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::ProcessBase*)proc);

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

size_t HandleSyscallMemoryUnMap(uintptr_t base, size_t length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1; /* Make sure it is in valid memory */

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::ProcessBase*)proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uintptr_t end = base + length; base < end; base += PAGE_SIZE) {
		procSpace->UnmapMemory((void*)base);
	}

	return 0;
}

size_t HandleSyscallMemoryInOut(const_userptr_t iorequests, size_t count) {
	if (count > 128) return -EINVALID;

	IORequest requests[count];
	CopyFromUser(requests, iorequests, count * sizeof(IORequest));
	ExecuteIORequest(requests, count);

	return 0;
}


size_t HandleSyscallProcExec(userptr_t executableBase, size_t executableSize) {
	uint8_t *heapAddr = (uint8_t*)Malloc(executableSize);

	CopyFromUser(heapAddr, executableBase, executableSize);
	
	size_t pid = LoadExecutableFile((uint8_t*)heapAddr, executableSize);
	PRINTK::PrintK("New process is PID: 0x%x\r\n", pid);

	Free(heapAddr);

	/* TODO: fix, Buggy, do not use
	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, pid, 0), PROC::ExecutableUnitState::P_READY);
	*/

	return 0;
}

size_t HandleSyscallProcFork(uintptr_t entrypoint) {
	KInfo *info = GetInfo();

	PROC::UserProcess *parentProc = NULL, *childProc = NULL;
	VMM::VirtualSpace *parentSpace = NULL, *childSpace = NULL;
	VMM::PageList *parentPages = NULL, *childPages = NULL;

	parentProc = (PROC::UserProcess*)(PROC::UserProcess*)PROC::GetProcess();
	parentSpace = GetVirtualSpace((PROC::ProcessBase*)(PROC::ProcessBase*)parentProc);
	parentPages = GetPageList((PROC::ProcessBase*)parentProc);

	parentSpace->Fork(childSpace, false);

	childPages = (VMM::PageList*)Malloc(parentPages->AllocatedSize);
	childPages->PageCount = parentPages->PageCount;
	childPages->AllocatedSize = parentPages->AllocatedSize;/*
	for (size_t currentPage = 0; currentPage < parentPages->PageCount; ++currentPage) {
		childPages->Pages[currentPage].IsCOW = true;
		if(parentPages->Pages[currentPage].Data.
		childPages->Pages[currentPage].Data.COW = (VMM::COWMetadata*)Malloc(sizeof(VMM::COWMetadata) + sizeof(uintptr_t));
		childPages->Pages[currentPage].Data.COW->PhysicalAddressOfOriginal = (uintptr_t)lastPhysicalPage;
		childPages->Pages[currentPage].Data.COW->PhysicalAddressOfCopy = 0;
		childPages->Pages[currentPage].Data.COW->VirtualReferences = 1;
		childPages->Pages[currentPage].Data.COW->VirtualAddresses[0] = (addr - addr % PAGE_SIZE);
	}*/

	childProc = (PROC::UserProcess*)PROC::CreateProcess((PROC::ProcessBase*)parentProc, PROC::ExecutableUnitType::PT_USER, childSpace, childPages, 0, 0);
	PROC::UserThread *thread = (PROC::UserThread*)PROC::CreateThread((PROC::ProcessBase*)childProc, entrypoint, 64 * 1024, 0, 0);

	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, thread);

	return 0;
}

size_t HandleSyscallProcReturn(size_t returnCode) {
	KInfo *info = GetInfo();

	uintptr_t kernelStack, userStack, userStackBase;
#if defined(ARCH_x64)
	asm volatile("mov %%gs:0, %0" : "=r"(kernelStack) : : "memory");
	asm volatile("mov %%gs:8, %0" : "=r"(userStack) : : "memory");
	asm volatile("mov %%gs:16, %0" : "=r"(userStackBase) : : "memory");
/*
	CPUStatus context;
	Memset(&context, 0, sizeof(context));
	context.RBX = *(uint64_t*)kernelStack;
	context.R12 = *(uint64_t*)(kernelStack - 8);
	context.R13 = *(uint64_t*)(kernelStack - 16);
	context.R14 = *(uint64_t*)(kernelStack - 24);
	context.R15 = *(uint64_t*)(kernelStack - 32);
	context.IretRIP = *(uint64_t*)(kernelStack - 48);
	context.IretRFLAGS = *(uint64_t*)(kernelStack - 56);
	context.IretRSP = userStack;
	
	context.IretCS = GDT_OFFSET_USER_CODE;
	context.IretSS = GDT_OFFSET_USER_CODE + 0x08;
				
	Memcpy(info->KernelScheduler->CurrentThread->Thread->Context, &context, sizeof(context));
*/
#endif

	PRINTK::PrintK("Returning in 0x%x: %d form 0x%x (0x%x)\r\n", kernelStack, returnCode, userStack, userStackBase); 

	//PROC::SetExecutableUnitState(info->KernelScheduler->CurrentThread->Thread, PROC::ExecutableUnitState::P_WAITING);

	PROC::PrintSchedulerStatus(info->KernelScheduler);

#if defined(ARCH_x64)
	asm volatile ("swapgs");
	asm volatile ("sti");
#endif
	while(true);

	return 0;
}

size_t HandleSyscallProcExit(size_t exitCode) {
	uintptr_t stack = 0;
	PRINTK::PrintK("Exiting: %d form 0x%x\r\n", exitCode, stack); 
	
	while(true);

	return 0;
}

size_t HandleSyscallProcWait(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallProcSendSig(size_t TODO) {
	(void)TODO;
	return 0;
}

size_t HandleSyscallModuleRegister(size_t vendorID, size_t productID) {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();

	info->KernelModuleManager->RegisterModule(proc, vendorID, productID);

	return 0;
}

size_t HandleSyscallModuleUnregister() {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod != NULL) info->KernelModuleManager->UnregisterModule(mod->GetVendor(), mod->GetProduct());
	
	return 0;
}

size_t HandleSyscallModuleBufferCreate(size_t size, size_t type, uint32_t *id) {
	KInfo *info = GetInfo();

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();

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

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::ProcessBase*)proc);
	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return 0;

	int result = info->KernelBufferManager->MapBuffer(mod->GetVendor(), mod->GetProduct(), id, procSpace, address);

	return result;
}

size_t HandleSyscallModuleBufferUnMap(uintptr_t address, uint32_t id) {
	(void)address;
	(void)id;
	return 0;
}

size_t HandleSyscallModuleBufferDelete(uint32_t id) {
	(void)id;
	return 0;
}

size_t HandleSyscallModuleMessageHandler(uintptr_t entry) {
	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	(void)proc;

	(void)entry;
	/* TODO: fix this
	proc->SetMessageThread(entry);
	*/

	return 0;
}

size_t HandleSyscallModuleMessageSend(uint32_t vendorID, uint32_t productID, void *data, size_t size) {
	if(size == 0) return -1;
	(void)vendorID;
	(void)productID;
	(void)data;
	(void)size;

	return 0;
}

size_t HandleSyscallModuleSectionRegister(const char *sectionName) {
	KInfo *info = GetInfo();

	char parsedSectionName[MAX_SECTION_LENGTH] = { 0 };
	Strncpy(parsedSectionName, sectionName, MAX_SECTION_LENGTH);

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return -1;

	info->KernelSectionManager->RegisterSectionDriver(parsedSectionName, mod->GetVendor(), mod->GetProduct());

	return 0;
}

size_t HandleSyscallModuleSectionGet(const char *sectionName, uint32_t *vendorID, uint32_t *productID) {
	KInfo *info = GetInfo();

	char parsedSectionName[MAX_SECTION_LENGTH] = { 0 };
	Strncpy(parsedSectionName, sectionName, MAX_SECTION_LENGTH);

	uint32_t newVendor, newProduct;

	info->KernelSectionManager->GetSectionDriver(parsedSectionName, &newVendor, &newProduct);
	
	*vendorID = newVendor;
	*productID = newProduct;

	return 0;
}

size_t HandleSyscallModuleSectionUnregister(const char *sectionName) {
	KInfo *info = GetInfo();

	char parsedSectionName[MAX_SECTION_LENGTH] = { 0 };
	Strncpy(parsedSectionName, sectionName, MAX_SECTION_LENGTH);

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();

	MODULE::Module *mod = info->KernelModuleManager->GetModule(proc->ID);
	if (mod == NULL) return 0;

	info->KernelSectionManager->UnregisterSectionDriver(parsedSectionName, mod->GetVendor(), mod->GetProduct());

	return 0;
}
