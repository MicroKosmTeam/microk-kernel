#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/queue.hpp>
#include <mm/pmm.hpp>

namespace PROC {
namespace Scheduler {

Queue<Process*> SchedulerQueue;
Process *kernelProcess;

void *stackone;
void *stacktwo;

void Entrypoint() {
	while(true) {
		PRINTK::PrintK("\r\nHello from thread.\r\n"
				"Current stack: 0x%x\r\n"
				"Next stack: 0x%x\r\n\r\n",
				(uint64_t)stackone,
				(uint64_t)stacktwo);
		void *tmp = stackone;
		stackone = stacktwo;
		stacktwo = tmp;

		SwitchStack(stacktwo, stackone);
	}
	
	while(true) asm volatile("hlt");
}

void InitStack( ) {

}

void Initialize() {
	KInfo *info = GetInfo();

	kernelProcess = CreateProcess(PT_KERNEL, PROC::Scheduler::Cycle, info->kernelVirtualSpace);
	SchedulerQueue.Init();
	SchedulerQueue.Push(kernelProcess);

	PRINTK::PrintK("Scheduler initialized.\r\n");

	stackone = PMM::RequestPage();
	stacktwo = PMM::RequestPage();
	memset(stackone, 0, 0x1000);
	memset(stacktwo, 0, 0x1000);

	InitializeStack(stackone, &Entrypoint);
	InitializeStack(stacktwo, &Entrypoint);

	PRINTK::PrintK("Stack initialized.\r\n");
	
	PRINTK::PrintK("Current stack: 0x%x\r\n"
			"Next stack: 0x%x\r\n",
			(uint64_t)info->kernelStack,
			(uint64_t)stackone);

	SwitchStack(info->kernelStack, stackone);

	PRINTK::PrintK("Done\r\n");

	while (true) asm volatile("hlt");
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
