#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/queue.hpp>
#include <mm/pmm.hpp>

namespace PROC {
namespace Scheduler {

Queue<Thread*> SchedulerQueue;
Process *kernelProcess;
Thread *currentThread;

void InitStack( ) {

}

void Initialize() {
	KInfo *info = GetInfo();

	kernelProcess = CreateProcess(PT_KERNEL, info->kernelVirtualSpace);

	SchedulerQueue.Init();

	PRINTK::PrintK("Scheduler initialized.\r\n");
}

void Yeild(Thread *thread) {
	KInfo *info = GetInfo();

	SwitchStack(&thread->Stack, &info->kernelStack);

	return;
}

void Pause() {

}

void AddCPU() {
	while (true) {
		Cycle();
	}
}

void Cycle() {
	KInfo *info = GetInfo();

	SchedulerQueue.Pop(&currentThread);
	if (currentThread == NULL) return;
	SwitchStack(&info->kernelStack, &currentThread->Stack);
	SchedulerQueue.Push(currentThread);
}

	
void StartKernelThread(uintptr_t entrypoint) {
	SchedulerQueue.Push(CreateThread(kernelProcess, entrypoint));
}

}
}
