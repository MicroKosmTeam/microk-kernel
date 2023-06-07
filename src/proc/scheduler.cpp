#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/panic.hpp>
#include <init/main.hpp>

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
}

void Scheduler::AddProcess(Process *process) {
	if (process == NULL) return;

	Processes.Push(process, process->GetPID());
}

Process *Scheduler::GetProcess(size_t PID) {
	Process *proc = Processes.Get(PID);
	
	return proc;
}

Process *Scheduler::GetRunningProcess() {
	return CurrentProcess;
}

#include <sys/user.hpp>
void Scheduler::SwitchToTask(size_t PID, size_t TID) {
	Process *proc = GetProcess(PID);
	if (proc == NULL) return;

	Thread *thread;
	if (TID == 0) thread = proc->GetMainThread();
	else thread = proc->GetThread(TID);

	if (thread == NULL) return;

	VMM::LoadVirtualSpace(proc->GetVirtualMemorySpace());
	
	void *stack = thread->GetStack();
	void *entry = thread->GetInstruction();

	CurrentProcess = proc;

	switch(proc->GetType()) {
		case PT_USER:
			EnterUserspace(entry, stack);
			break;
		case PT_KERNEL:
			OOPS("Kernel task switching not yet implemented");
			break;
	}
}

}
