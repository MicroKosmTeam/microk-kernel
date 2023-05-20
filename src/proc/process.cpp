#include <proc/process.hpp>
#include <mm/memory.hpp>
#include <arch/x64/cpu/stack.hpp>

#include <sys/printk.hpp>
#include <mm/pmm.hpp>

namespace PROC {
static uint64_t CurrentPID;

static inline uint64_t RequestPID() {
	CurrentPID++;

	return CurrentPID;
}

static inline uint64_t RequestTID(Process *process) {
	process->LastTID++;

	return process->LastTID;
}

Process *CreateProcess(ProcessType type, VMM::VirtualSpace *vms) {
	Process *newProcess = new Process;
	newProcess->PID = RequestPID();
	newProcess->State = P_READY;
	newProcess->Type = type;

	newProcess->VirtualMemorySpace = vms;
	newProcess->HighestFree = 0x00007FFFFFFFF000;

	newProcess->LastTID = 0;
	newProcess->ThreadNumber = 0;
	newProcess->MainThread = NULL;

	switch(type) {
		case PT_KERNEL:
			break;
		case PT_USER:
			break;
		case PT_VM:
			break;
		case PT_REALTIME:
			break;
	}

	return newProcess;
}

void DeleteProcess(Process *process) {

}

Thread *CreateThread(Process *process, uintptr_t entrypoint) {
	Thread *newThread = new Thread;

	newThread->TID = RequestTID(process);
	newThread->Owner = process;
	newThread->Entrypoint = entrypoint;
	newThread->State = P_READY;

	size_t stackSize = (512 * 1024);
	stackSize -= stackSize % 16;

	// This is obviously wrong, but it is the only thing that works
	void *stackAddress = Malloc(stackSize) + stackSize;
	stackAddress -= (uintptr_t)stackAddress % 16;
	newThread->Stack = stackAddress;
	
	/*process->HighestFree -= process->HighestFree % 16;

	for (uintptr_t i = process->HighestFree - stackSize; i < process->HighestFree; i+= 0x1000) {
		process->VirtualMemorySpace->MapMemory((void*)PMM::RequestPage(), (void*)i, 0);
	}

	memset(process->HighestFree - stackSize, 0, stackSize);

	/* We subtract so to leave some space for the first save context */
	//newThread->Stack = process->HighestFree - sizeof(SaveContext);
	newThread->Stack -= sizeof(SaveContext);
	//process->HighestFree = newThread->Stack - 0x1000;

	InitializeStack(newThread, entrypoint);
	
	process->Threads.Push(newThread);
	process->ThreadNumber++;

	if (process->MainThread == NULL) process->MainThread = newThread;

	Thread *complete = process->Threads.Get(process->ThreadNumber - 1);

	return complete;
}

void DeleteThread(Thread *thread) {
	if (thread == NULL) return;

}
}
