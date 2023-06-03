#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <sys/queue.hpp>
#include <mm/pmm.hpp>

#include <sys/panic.hpp>
namespace PROC {
namespace Scheduler {

void Initialize() {
	OOPS("Scheduler not implemented");
	KInfo *info = GetInfo();

	//info->kernelProcess = CreateProcess(PT_KERNEL, info->kernelVirtualSpace);
	//info->kernelProcess->MainThread = CreateThread(info->kernelProcess, NULL);
}

void Yeild(Thread *thread) {

}

void Pause() {

}

void AddCPU() {

}

void Cycle() {

}

	
void StartKernelThread(uintptr_t entrypoint) {

}

}
}
