#include <proc/process.hpp>
#include <mm/memory.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>
#include <mm/pmm.hpp>
#include <stdint.h>
#include <stddef.h>
#include <cdefs.h>

namespace PROC {
static size_t CurrentPID = 0; /* PIDs start at 0 */

static inline size_t RequestPID() {
	return CurrentPID++;
}

static uintptr_t GetHighestFree() {
#if defined(ARCH_x64)
	return 0x800000000000 - PAGE_SIZE;
#endif
	return -1;
}

static ExecutableUnitHeader *CreateExecutableUnitHeader(ProcessBase *parent, ExecutableUnitType type, bool isThread, uint8_t priority, uint16_t flags) {
	ExecutableUnitHeader *unit = NULL;
	if(isThread) {
		switch(type) {
			case ExecutableUnitType::PT_KERNEL:
				unit = (ExecutableUnitHeader*)(new KernelThread);
				break;
			case ExecutableUnitType::PT_USER:
				unit = (ExecutableUnitHeader*)(new UserThread);
				break;
			case ExecutableUnitType::PT_REALTIME:
				unit = (ExecutableUnitHeader*)(new RealtimeThread);
				break;
			case ExecutableUnitType::PT_VM:
				unit = (ExecutableUnitHeader*)(new VMThread);
				break;
		}
	} else {
		switch(type) {
			case ExecutableUnitType::PT_KERNEL:
				unit = (ExecutableUnitHeader*)(new KernelProcess);
				break;
			case ExecutableUnitType::PT_USER:
				unit = (ExecutableUnitHeader*)(new UserProcess);
				break;
			case ExecutableUnitType::PT_REALTIME:
				unit = (ExecutableUnitHeader*)(new RealtimeProcess);
				break;
			case ExecutableUnitType::PT_VM:
				unit = (ExecutableUnitHeader*)(new VMProcess);
				break;
		}
	}

	unit->Priority = priority;
	unit->Flags = flags;
	unit->IsThread = isThread;
	unit->Parent = parent;
	unit->Type = type;
	unit->State = ExecutableUnitState::P_WAITING;

	return unit;
}

static size_t RequestTID(ProcessBase *parent) {
	if(parent == NULL || parent->IsThread) return 0;

	return parent->Threads.ThreadIDBase++;
}

ProcessBase *CreateProcess(ProcessBase *parent, ExecutableUnitType type, VMM::VirtualSpace *virtualMemorySpace, VMM::PageList *pageList, uint8_t priority, uint16_t flags) {
	KInfo *info = GetInfo();
	ProcessBase *process = (ProcessBase*)CreateExecutableUnitHeader(parent, type, false, priority, flags);
	if(process == NULL) return NULL;

	process->ID = RequestPID();
	process->VirtualMemorySpace = virtualMemorySpace;
	process->ExecutablePageList = pageList;

	switch(type) {
		case ExecutableUnitType::PT_KERNEL:
			break;
		case ExecutableUnitType::PT_USER: {
			UserProcess *userProcess = (UserProcess*)process;

			userProcess->HighestFree = GetHighestFree();

			userProcess->UserTaskBlock = (UserTCB*)PMM::RequestPage();
			UserTCB *tcb = (UserTCB*)((uintptr_t)userProcess->UserTaskBlock + info->HigherHalfMapping);
			Memset(tcb, 0, PAGE_SIZE);

			PopulateUserTCB(tcb);

			userProcess->HighestFree -= PAGE_SIZE;
			VMM::MapMemory(userProcess->VirtualMemorySpace,
				       (void*)userProcess->UserTaskBlock,
				       (void*)userProcess->HighestFree,
				       VMM::VirtualMemoryFlags::VMM_PRESENT |
				       VMM::VirtualMemoryFlags::VMM_READWRITE |
				       VMM::VirtualMemoryFlags::VMM_USER |
				       VMM::VirtualMemoryFlags::VMM_NOEXECUTE);
			}
			break;
		case ExecutableUnitType::PT_REALTIME:
			break;
		case ExecutableUnitType::PT_VM:
			break;

	}


	return process;
}

int DeleteProcess(ProcessBase *process) {
	if(process == NULL) return -1;
	if(process->State != ExecutableUnitState::P_DONE) {
		/* The process hasn't been properly dismantled 
		 * We'll continue anyway, but this will cause issues
		 */
		OOPS("Process not properly dismantled");
	}

	/* TODO: delete process */

	return 0;
}

ThreadBase *CreateThread(ProcessBase *parent, uintptr_t entrypoint, size_t stackSize, uint8_t priority, uint16_t flags) {
	if(parent == NULL) return NULL;

	ThreadBase *thread = (ThreadBase*)CreateExecutableUnitHeader(parent, parent->Type, true, priority, flags);
	if(thread == NULL) return NULL;
	
	KInfo *info = GetInfo();

	/* Initializing thread variables */
	thread->ID = RequestTID(parent);
	thread->Context = new CPUStatus;
	Memset(thread->Context, 0, sizeof(CPUStatus));

	/* Adding the thread to the list in the parent */
	ThreadBase *precedingThread = parent->Threads.Tail;
	thread->Next = NULL;
	++parent->Threads.ThreadCount;

	if(precedingThread == NULL) {
		parent->Threads.Head = thread;
		parent->Threads.Tail = thread;

		thread->Previous = NULL;
	} else {
		parent->Threads.Tail = thread;
		precedingThread->Next = thread;

		thread->Previous = precedingThread;
	}

	switch(parent->Type) {
		case ExecutableUnitType::PT_KERNEL: {
			KernelThread *kernelThread = (KernelThread*)thread;
			KernelProcess *kernelParent = (KernelProcess*)parent;
			
			size_t highestFree = kernelParent->HighestFree;
			VMM::VirtualSpace *space = kernelParent->VirtualMemorySpace;

			for (uintptr_t i = highestFree - stackSize; i < highestFree; i+= PAGE_SIZE) {
				void *physical = PMM::RequestPage();
				VMM::MapMemory(space, physical, (void*)i,
					       VMM::VirtualMemoryFlags::VMM_PRESENT |
					       VMM::VirtualMemoryFlags::VMM_READWRITE |
					       VMM::VirtualMemoryFlags::VMM_NOEXECUTE);
				Memset((void*)((uintptr_t)physical + info->HigherHalfMapping), 0, PAGE_SIZE);
			}

			kernelThread->KernelStack = highestFree;
			kernelParent->HighestFree -= stackSize + (PAGE_SIZE - stackSize % PAGE_SIZE);

			kernelThread->Context->IretRIP = entrypoint;
			kernelThread->Context->IretRSP = highestFree;
			kernelThread->Context->IretRFLAGS = 0x0202;
			
			kernelThread->Context->IretCS = GDT_OFFSET_KERNEL_CODE;
			kernelThread->Context->IretSS = GDT_OFFSET_KERNEL_CODE + 0x8;
			}
			break;
		case ExecutableUnitType::PT_USER: {
			UserThread *userThread = (UserThread*)thread;
			UserProcess *userParent = (UserProcess*)parent;

			size_t highestFree = userParent->HighestFree;
			VMM::VirtualSpace *space = userParent->VirtualMemorySpace;

			for (uintptr_t i = highestFree - stackSize; i < highestFree; i+= PAGE_SIZE) {
				void *physical = PMM::RequestPage();
				VMM::MapMemory(space, physical, (void*)i,
					       VMM::VirtualMemoryFlags::VMM_PRESENT |
					       VMM::VirtualMemoryFlags::VMM_READWRITE |
					       VMM::VirtualMemoryFlags::VMM_USER |
					       VMM::VirtualMemoryFlags::VMM_NOEXECUTE);
				Memset((void*)((uintptr_t)physical + info->HigherHalfMapping), 0, PAGE_SIZE);
			}

			userThread->Context->IretRIP = entrypoint;
			userThread->Context->IretRSP = highestFree;
			userThread->Context->IretRFLAGS = 0x0202;
			
			userThread->Context->IretCS = GDT_OFFSET_USER_CODE;
			userThread->Context->IretSS = GDT_OFFSET_USER_CODE + 0x8;

			userThread->UserStack = highestFree;
			userParent->HighestFree -= stackSize + (PAGE_SIZE - stackSize % PAGE_SIZE);

			highestFree = userParent->HighestFree;

			const size_t kernelStackSize = 64 * 1024;
			for (uintptr_t i = highestFree - kernelStackSize; i < highestFree; i+= PAGE_SIZE) {
				void *physical = PMM::RequestPage();
				VMM::MapMemory(space, physical, (void*)i,
					       VMM::VirtualMemoryFlags::VMM_PRESENT |
					       VMM::VirtualMemoryFlags::VMM_READWRITE |
					       VMM::VirtualMemoryFlags::VMM_NOEXECUTE);
				Memset((void*)((uintptr_t)physical + info->HigherHalfMapping), 0, PAGE_SIZE);
			}

			userThread->KernelStack = highestFree;
			userParent->HighestFree -= stackSize + (PAGE_SIZE - stackSize % PAGE_SIZE);
			}
			break;
		case ExecutableUnitType::PT_REALTIME:
			break;
		case ExecutableUnitType::PT_VM:
			break;
	}

	return thread;
}


ThreadBase *FindThread(ProcessBase *process, size_t id) {
	if(process == NULL) return NULL;
	ThreadList *list = &process->Threads;

	ThreadBase *current = list->Head;

	while(current != list->Tail) {
		if (current->ID == id) return current;
	}

	return NULL;

}

int DeleteThread(ThreadBase *thread) {
	if(thread == NULL) return -1;
	if (thread->State != ExecutableUnitState::P_DONE) {
		/* The process hasn't been properly dismantled 
		 * We'll continue anyway, but this will cause issues
		 */
		OOPS("Thread not properly dismantled");
	}

	/* TODO: delete thread */

	return 0;
}
	
int SetExecutableUnitState(ExecutableUnitHeader *unit, ExecutableUnitState state) {
	if(unit == NULL) return -1;

	unit->State = state;

	/* TODO: actually set state */

	return 0;
}

void PopulateUserTCB(UserTCB *tcb) {
	if(tcb == NULL) return;
	tcb->Signature[0] = 'U';
	tcb->Signature[1] = 'T';
	tcb->Signature[2] = 'C';
	tcb->Signature[3] = 'B';

	tcb->Revision = 1;
	tcb->Checksum = 0;
	
	tcb->SystemTables = 2;
	tcb->ServiceTables = 1;

	tcb->SystemTableListOffset = sizeof(UserTCB);
	
	TableListElement *systemTableList = (TableListElement*)((uintptr_t)tcb + tcb->SystemTableListOffset);

	systemTableList[0].Signature[0] = 'K';
	systemTableList[0].Signature[1] = 'B';
	systemTableList[0].Signature[2] = 'S';
	systemTableList[0].Signature[3] = 'T';
	systemTableList[0].TablePointer = 0;

	systemTableList[1].Signature[0] = 'P';
	systemTableList[1].Signature[1] = 'T';
	systemTableList[1].Signature[2] = 'S';
	systemTableList[1].Signature[3] = 'T';
	systemTableList[1].TablePointer = 0;

	size_t systemTableListSize = tcb->SystemTables * sizeof(TableListElement);
	tcb->ServiceTableListOffset = tcb->SystemTableListOffset + systemTableListSize;

	TableListElement *serviceTableList = (TableListElement*)((uintptr_t)tcb + tcb->ServiceTableListOffset);

	serviceTableList[0].Signature[0] = 'D';
	serviceTableList[0].Signature[1] = 'T';
	serviceTableList[0].Signature[2] = 'I';
	serviceTableList[0].Signature[3] = 'T';
	serviceTableList[0].TablePointer = 0;

	uint8_t checksumDifference = 0;
	uint8_t *tcbByte = (uint8_t*)tcb;

	while((uintptr_t)tcbByte < (uintptr_t)tcb + PAGE_SIZE) {
		checksumDifference += *tcbByte++;
	}

	tcb->Checksum = 0x100 - checksumDifference;
}

}
