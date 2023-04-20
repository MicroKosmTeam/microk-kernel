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

	newProcess->ThreadNumber = 0;
	newProcess->LastTID = 0;
	newProcess->MainThread = NULL;

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
	newThread->Stack = PMM::RequestPages(16);

	InitializeStack(newThread->Stack, entrypoint);
	
	process->Threads.Push(newThread);
	process->ThreadNumber++;

	return process->Threads.Get(process->ThreadNumber - 1);
}

void DeleteThread(Thread *thread) {

}
}
