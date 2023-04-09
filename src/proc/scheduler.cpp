#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <sys/queue.hpp>

namespace PROC {
namespace Scheduler {

Queue<Process*> SchedulerQueue;
Process *kernelProcess;

void Initialize() {
	kernelProcess = CreateProcess(PT_KERNEL, PROC::Scheduler::Cycle);
	SchedulerQueue.Init();
	SchedulerQueue.Push(kernelProcess);
}

void Pause() {

}

void AddCPU() {
}

void Cycle() {
	Process *currentProcess;
	SchedulerQueue.Pop(&currentProcess);
	SchedulerQueue.Push(currentProcess);

	// Execute the process here
}
	
void StartKernelThread(uintptr_t entrypoint) {
	CreateThread(kernelProcess, entrypoint);

	void (*Entrypoint)() = entrypoint;
	Entrypoint();
}

}
}
