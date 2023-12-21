#include <mm/pmm.hpp>
#include <sys/io.hpp>
#include <sys/file.hpp>
#include <mm/string.hpp>
#include <mm/memory.hpp>
#include <sys/panic.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/loader.hpp>
#include <sys/syscall.hpp>
#include <proc/helpers.hpp>

__attribute__((aligned(PAGE_SIZE))) SyscallFunctionCallback SyscallVector[SYSCALL_VECTOR_END];

void InitSyscalls() {
	Memclr(SyscallVector, SYSCALL_VECTOR_END * sizeof(SyscallFunctionCallback));

	PRINTK::PrintK(PRINTK_DEBUG "Initializing system call API.\r\n");
	SyscallVector[SYSCALL_DEBUG_PRINTK] = (SyscallFunctionCallback)(void*)HandleSyscallDebugPrintK;/*
	SyscallVector[SYSCALL_MEMORY_VMALLOC] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryVMAlloc;
	
	SyscallVector[SYSCALL_MEMORY_MMAP] = (SyscallFunctionCallback)(void*)HandleSyscallMemoryMMap;

	SyscallVector[SYSCALL_IPC_MESSAGE_QUEUE] = (SyscallFunctionCallback)(void*)HandleSyscallIPCQueue;
	SyscallVector[SYSCALL_IPC_MESSAGE_SEND] = (SyscallFunctionCallback)(void*)HandleSyscallIPCMessageSend;
	SyscallVector[SYSCALL_IPC_MESSAGE_RECEIVE] = (SyscallFunctionCallback)(void*)HandleSyscallIPCMessageReceive;
	
	SyscallVector[SYSCALL_PROC_EXIT] = (SyscallFunctionCallback)(void*)HandleSyscallProcExit;*/

}

extern "C" usize HandleSyscall(usize syscallNumber, usize arg1, usize arg2, usize arg3, usize arg4, usize arg5, usize arg6) {
	if(syscallNumber <= SYSCALL_VECTOR_START ||
	   syscallNumber >= SYSCALL_VECTOR_END ||
	   SyscallVector[syscallNumber] == NULL) return -ENOTPRESENT;

	return SyscallVector[syscallNumber](arg1, arg2, arg3, arg4, arg5, arg6);
}


usize HandleSyscallDebugPrintK(const_userptr_t userString) {
	char string[MAX_PRINTK_SYSCALL_MESSAGE_LENGTH + 1] = { '\0' };
	CopyStringFromUser(string, userString, MAX_PRINTK_SYSCALL_MESSAGE_LENGTH);

	PRINTK::PrintK(PRINTK_DEBUG "%s", string);

	return 0;
}
/*
usize HandleSyscallMemoryVMAlloc(const_userptr_t userBase, usize length, usize flags) {
	if (CheckUserMemory(userBase, length)) {
		PANIC("Bad memory");
		return -EBADREQUEST;
	}

	uptr base = (uptr)userBase;

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::Process*)proc);
	
	PRINTK::PrintK(PRINTK_DEBUG "Calling VMAlloc for PID %d. Base: 0x%x, Length: %d bytes, Flags: 0x%x.\r\n", proc->ID, base, length, flags);

	VMM::VMAlloc(procSpace, base, length, VMM_FLAGS_USER_DATA);

	PRINTK::PrintK(PRINTK_DEBUG "Executing VMAlloc for PID %d completed successfully.\r\n", proc->ID);

	return 0;
}

usize HandleSyscallMemoryMMap(const_userptr_t userSrc, const_userptr_t userDest, usize length, usize flags) {
	if (CheckUserMemory(userSrc, length) || CheckUserMemory(userDest, length)) {
		PANIC("Bad memory");
		return -EBADREQUEST;
	}

	uptr src = (uptr)userSrc;
	uptr dest = (uptr)userDest;

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::Process*)proc);

	VMM::MMap(procSpace, src, dest, length, VMM::ConvertUserFlags(flags));

	return 0;
}

usize HandleSyscallIPCQueue(userptr_t userCtlStruct) {
	KInfo *info = GetInfo();
	PROC::QueueOperationStruct ctlStruct;

	CopyFromUser(&ctlStruct, userCtlStruct, sizeof(ctlStruct));

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	if (int ret = PROC::IPCMessageQueueCtl(info->KernelMessageManager, proc, &ctlStruct)) {
		return ret;
	}

	CopyToUser(&ctlStruct, userCtlStruct, sizeof(ctlStruct));

	return 0;
}

usize HandleSyscallIPCMessageSend(usize queueID, const_userptr_t userMessagePointer, usize messageLength, usize messageType, usize messageFlags) {	
	KInfo *info = GetInfo();

	u8 *messagePointer = (u8*)Malloc(messageLength);
	CopyFromUser(messagePointer, userMessagePointer, messageLength);

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();

	if (int ret = PROC::IPCMessageSend(info->KernelMessageManager, queueID, proc, messagePointer, messageLength, messageType, messageFlags)) {
		Free(messagePointer);

		return ret;
	}
	
	Free(messagePointer);

	return 0;
}

usize HandleSyscallIPCMessageReceive(usize queueID, userptr_t userMessageBufferPointer, usize maxMessageLength, usize messageType, usize messageFlags) {
	KInfo *info = GetInfo();

	u8 *messageBufferPointer = (u8*)Malloc(maxMessageLength);

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	if (int ret = PROC::IPCMessageReceive(info->KernelMessageManager, queueID, proc, messageBufferPointer, maxMessageLength, messageType, messageFlags)) {
		Free(messageBufferPointer);
		return ret;
	}

	CopyToUser(messageBufferPointer, userMessageBufferPointer, maxMessageLength);

	Free(messageBufferPointer);

	return 0;
}


usize HandleSyscallProcExit(usize exitCode) {
	PRINTK::PrintK(PRINTK_DEBUG "Exiting: %d\r\n", exitCode); 
	
	while(true);

	return 0;
}
*/

#ifdef UNDEF

usize HandleSyscallMemoryPMAlloc(userptr_t baseDestination, usize length, usize flags) {
	(void) flags;

	uptr base = 0;
	usize roundedLength = length / PAGE_SIZE + 1;

	if (length % PAGE_SIZE) {
		base = (uptr)PMM::RequestPages(roundedLength);
	} else {
		base = (uptr)PMM::RequestPage();
	}

	CopyToUser(&base, baseDestination, sizeof(uptr));

	return 0;
}

usize HandleSyscallMemoryVMFree(const_userptr_t userBase, usize length) {
	if(int code = CheckUserMemory(userBase, length) != 0)
		return code;

	uptr base = (uptr)userBase;

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::Process*)proc);

	if (base % PAGE_SIZE) {
		base -= base % PAGE_SIZE;
	}

	if (length % PAGE_SIZE) {
		length += PAGE_SIZE - length % PAGE_SIZE;
	}

	for (uptr vaddr = base; vaddr < base + length; vaddr += PAGE_SIZE) {
		uptr paddr = (uptr)procSpace->GetPhysicalAddress((void*)vaddr);
		procSpace->UnmapMemory((void*)vaddr);
		if (paddr == 0) return -1;

		PMM::FreePage((void*)paddr);
	}

	return 0;
}


usize HandleSyscallMemoryUnMap(uptr base, usize length) {
	if (base <= 0x1000 || base + length >= 0x00007FFFFFFFF000)
		return -1; /* Make sure it is in valid memory */

	PROC::UserProcess *proc = (PROC::UserProcess*)PROC::GetProcess();
	VMM::VirtualSpace *procSpace = GetVirtualSpace((PROC::Process*)proc);

	if (base % PAGE_SIZE) base -= base % PAGE_SIZE;
	if (length % PAGE_SIZE) length += PAGE_SIZE - length % PAGE_SIZE;

	for (uptr end = base + length; base < end; base += PAGE_SIZE) {
		procSpace->UnmapMemory((void*)base);
	}

	return 0;
}

usize HandleSyscallMemoryInOut(const_userptr_t iorequests, usize count) {
	if (count > 128) return -EINVALID;

	IORequest requests[count];
	CopyFromUser(requests, iorequests, count * sizeof(IORequest));
	ExecuteIORequest(requests, count);

	return 0;
}


usize HandleSyscallProcExec(userptr_t executableBase, usize executableSize) {
	u8 *heapAddr = (u8*)Malloc(executableSize);

	CopyFromUser(heapAddr, executableBase, executableSize);
	
	usize pid = LoadExecutableFile((u8*)heapAddr, executableSize);
	PRINTK::PrintK(PRINTK_DEBUG "New process is PID: 0x%x\r\n", pid);

	Free(heapAddr);

	/* TODO: fix, Buggy, do not use
	PROC::SetExecutableUnitState(PROC::GetThread(info->KernelScheduler, pid, 0), PROC::ExecutableUnitState::P_READY);
	*/

	return 0;
}

usize HandleSyscallProcFork() {
	KInfo *info = GetInfo();

	uptr entrypoint = 0;

	PROC::UserProcess *parentProc = NULL, *childProc = NULL;
	VMM::VirtualSpace *parentSpace = NULL, *childSpace = NULL;
	VMM::PageList *parentPages = NULL, *childPages = NULL;

	parentProc = (PROC::UserProcess*)(PROC::UserProcess*)PROC::GetProcess();
	parentSpace = GetVirtualSpace((PROC::Process*)(PROC::Process*)parentProc);
	parentPages = GetPageList((PROC::Process*)parentProc);

	parentSpace->Fork(childSpace, false);

	childPages = (VMM::PageList*)Malloc(parentPages->AllocatedSize);
	childPages->PageCount = parentPages->PageCount;
	childPages->AllocatedSize = parentPages->AllocatedSize;/*
	for (usize currentPage = 0; currentPage < parentPages->PageCount; ++currentPage) {
		childPages->Pages[currentPage].IsCOW = true;
		if(parentPages->Pages[currentPage].Data.
		childPages->Pages[currentPage].Data.COW = (VMM::COWMetadata*)Malloc(sizeof(VMM::COWMetadata) + sizeof(uptr));
		childPages->Pages[currentPage].Data.COW->PhysicalAddressOfOriginal = (uptr)lastPhysicalPage;
		childPages->Pages[currentPage].Data.COW->PhysicalAddressOfCopy = 0;
		childPages->Pages[currentPage].Data.COW->VirtualReferences = 1;
		childPages->Pages[currentPage].Data.COW->VirtualAddresses[0] = (addr - addr % PAGE_SIZE);
	}*/

	childProc = (PROC::UserProcess*)PROC::CreateProcess((PROC::Process*)parentProc, PROC::ExecutableUnitType::PT_USER, childSpace, childPages, 0, 0);
	PROC::UserThread *thread = (PROC::UserThread*)PROC::CreateThread((PROC::Process*)childProc, entrypoint, 64 * 1024, 0, 0);

	PROC::AddThreadToQueue(info->KernelScheduler, SCHEDULER_RUNNING_QUEUE, thread);

	return 0;
}

usize HandleSyscallProcReturn(usize returnCode) {
	KInfo *info = GetInfo();

	uptr kernelStack, userStack, userStackBase;
#if defined(__x86_64__)
	asm volatile("mov %%gs:0, %0" : "=r"(kernelStack) : : "memory");
	asm volatile("mov %%gs:8, %0" : "=r"(userStack) : : "memory");
	asm volatile("mov %%gs:16, %0" : "=r"(userStackBase) : : "memory");
/*
	CPUStatus context;
	Memclr(&context, sizeof(context));
	context.RBX = *(u64*)kernelStack;
	context.R12 = *(u64*)(kernelStack - 8);
	context.R13 = *(u64*)(kernelStack - 16);
	context.R14 = *(u64*)(kernelStack - 24);
	context.R15 = *(u64*)(kernelStack - 32);
	context.IretRIP = *(u64*)(kernelStack - 48);
	context.IretRFLAGS = *(u64*)(kernelStack - 56);
	context.IretRSP = userStack;
	
	context.IretCS = GDT_OFFSET_USER_CODE;
	context.IretSS = GDT_OFFSET_USER_CODE + 0x08;
				
	Memcpy(info->KernelScheduler->CurrentThread->Thread->Context, &context, sizeof(context));
*/
#endif

	PRINTK::PrintK(PRINTK_DEBUG "Returning in 0x%x: %d form 0x%x (0x%x)\r\n", kernelStack, returnCode, userStack, userStackBase); 

	//PROC::SetExecutableUnitState(info->KernelScheduler->CurrentThread->Thread, PROC::ExecutableUnitState::P_WAITING);

	PROC::PrintSchedulerStatus(info->KernelScheduler);

#if defined(__x86_64__)
	asm volatile ("swapgs");
	asm volatile ("sti");
#endif
	while(true);

	return 0;
}
#endif


