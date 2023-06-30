#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/panic.hpp>
#include <init/main.hpp>
#include <sys/user.hpp>

namespace PROC {
Scheduler::Scheduler() {
	KInfo *info = GetInfo();
	
	Processes.Init();

	info->kernelProcess = new Process(PT_KERNEL, info->kernelVirtualSpace);
	size_t mainTID = info->kernelProcess->CreateThread(128 * 1024, RestInit);

	info->kernelProcess->SetMainThread(mainTID);
	info->kernelProcess->GetMainThread()->SetState(PROC::P_READY);

	info->kernelProcess->SetProcessState(PROC::P_READY);

	AddProcess(info->kernelProcess);

	CurrentProcess = info->kernelProcess;
}

void Scheduler::AddProcess(Process *process) {
	if (process == NULL) return;

	size_t pid = process->GetPID();

	Processes.Push(process, pid);

	if (pid > MaxPID) MaxPID = pid;
}

Process *Scheduler::GetProcess(size_t PID) {
	if(PID > MaxPID) return NULL;

	Process *proc = Processes.Get(PID);
	
	return proc;
}

Process *Scheduler::GetRunningProcess() {
	return CurrentProcess;
}

void Scheduler::SwitchToTask(size_t PID, size_t TID) {
	Process *proc = GetProcess(PID);
	if (proc == NULL) return;

	Thread *thread;
	if (TID == 0) thread = proc->GetMainThread();
	else thread = proc->GetThread(TID);

	if (thread == NULL) return;

	void *stack = thread->GetStack();
	void *entry = thread->GetInstruction();

	CurrentProcess = proc;

	ProcessType type = proc->GetType();
	
	VMM::LoadVirtualSpace(proc->GetVirtualMemorySpace());

	switch(type) {
		case PT_USER:
			EnterUserspace(entry, stack);
			break;
		case PT_KERNEL:
			break;
	}

	OOPS("Failed to task switch");
	while(true);
}

}
