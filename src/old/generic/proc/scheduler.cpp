#include <proc/scheduler.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/panic.hpp>
#include <sys/math.hpp>
#include <mm/pmm.hpp>

namespace PROC {
Scheduler *InitializeScheduler(Scheduler *parent) {
	KInfo *info = GetInfo();

	Scheduler *scheduler = (Scheduler*)MEM::SLAB::Alloc(info->SchedulerCache);

	scheduler->ParentScheduler = parent;
	scheduler->ProcessSlabCache = MEM::SLAB::InitializeSlabCache(info->KernelSlabAllocator, sizeof(Process));
	scheduler->ThreadSlabCache = MEM::SLAB::InitializeSlabCache(info->KernelSlabAllocator, sizeof(Thread));

	scheduler->RunningThread = NULL;

	return scheduler;
}

void DeinitializeScheduler(Scheduler *scheduler) {
	MEM::SLAB::FreeSlabCache(scheduler->ProcessSlabCache);
	MEM::SLAB::FreeSlabCache(scheduler->ThreadSlabCache);

	Free(scheduler);
}
	
void Tick(Scheduler *scheduler, u8 *context, usize contextSize) {
	if (scheduler->RunningThread != NULL) {
		Memcpy(scheduler->RunningThread->Context, context, contextSize);

		PushListTail(PopListHead(&scheduler->RunningQueue), &scheduler->RunningQueue);
	}
	
	scheduler->RunningThread = (Thread*)GetListHead(&scheduler->RunningQueue);
	
	if (scheduler->RunningThread != NULL) {
		Memcpy(context, scheduler->RunningThread->Context, contextSize);
	}
}

Process *CreateProcess(Scheduler *scheduler, ExecutableUnitType type, VMM::VirtualSpace *space) {
	Process *proc = (Process*)MEM::SLAB::Alloc(scheduler->ProcessSlabCache);
	
	proc->Type = type;

	proc->Domain = scheduler->Domain;
	proc->VirtualMemorySpace = space;

	MEM::MEMBLOCK::MemblockRegion *highestFree = MEM::MEMBLOCK::FindFreeRegion(space->VirtualMemoryLayout, PAGE_SIZE, false);
	proc->HighestFree = highestFree->Base + highestFree->Length;

	PRINTK::PrintK(PRINTK_DEBUG "Highest free: 0x%x\r\n", proc->HighestFree);

	proc->ThreadCount = 0;
	proc->ThreadIDBase = 0;

	switch (type) {
		case ExecutableUnitType::PT_KERNEL:
			break;
		case ExecutableUnitType::PT_USER:
			break;
		default:
			PRINTK::PrintK(PRINTK_ERROR "Unknown process type: 0x%x\r\n", type);
			OOPS("Unknown process type");
			MEM::SLAB::Free(scheduler->ProcessSlabCache, proc);
			return NULL;
	}

	return proc;

}

Thread *CreateThread(Scheduler *scheduler, Process *parent, uptr entry) {
	Thread *thread = (Thread*)MEM::SLAB::Alloc(scheduler->ThreadSlabCache);

	thread->Type = parent->Type;

	thread->Parent = parent;

	uptr contextPage = VMM::PhysicalToVirtual((uptr)PMM::RequestPage());
	Memclr((void*)contextPage, PAGE_SIZE);
	
	thread->Context = (u8*)contextPage;
/*
	parent->HighestFree -= PAGE_SIZE;
	VMM::MapPage(parent->VirtualMemorySpace, VMM::VirtualToPhysical(contextPage), parent->HighestFree, VMM_FLAGS_KERNEL_DATA);
	thread->Context = (usize*)parent->HighestFree;

	MEM::MEMBLOCK::MemblockRegion *contextRegion =
		MEM::MEMBLOCK::AddRegion(parent->VirtualMemorySpace->VirtualMemoryLayout,
				         parent->HighestFree, PAGE_SIZE, MEMMAP_KERNEL_CONTEXT);
	(void)contextRegion;
*/

	thread->KernelStack = parent->HighestFree;
	parent->HighestFree -= KERNEL_STACK_SIZE;

	MEM::MEMBLOCK::MemblockRegion *kernelStackRegion =
		VMM::VMAlloc(parent->VirtualMemorySpace, parent->HighestFree,
			     KERNEL_STACK_SIZE, VMM_FLAGS_KERNEL_DATA);

	kernelStackRegion->Type = MEMMAP_KERNEL_STACK;
	
	parent->HighestFree -= PAGE_SIZE;

	switch(parent->Type) {
		case ExecutableUnitType::PT_KERNEL:
			DEV::CPU::UpdateCPUContext(thread->Context, thread->KernelStack , entry, 0, 0, false);
			break;
		case ExecutableUnitType::PT_USER: {
			thread->UserThread.UserStack = parent->HighestFree;
			parent->HighestFree -= KERNEL_STACK_SIZE;

			MEM::MEMBLOCK::MemblockRegion *userStackRegion = 
				VMM::VMAlloc(parent->VirtualMemorySpace, parent->HighestFree,
					     KERNEL_STACK_SIZE, VMM_FLAGS_USER_DATA);

			userStackRegion->Type = MEMMAP_KERNEL_STACK;
	
			parent->HighestFree -= PAGE_SIZE;

			DEV::CPU::UpdateCPUContext(thread->Context, thread->UserThread.UserStack, entry, 0, 0, true);

			}
			break;
		default:
			PRINTK::PrintK(PRINTK_ERROR "Unknown process type: 0x%x\r\n", parent->Type);
			OOPS("Unknown process type");
			MEM::SLAB::Free(scheduler->ProcessSlabCache, thread);
			return NULL;

	
	}

	MEM::MEMBLOCK::ListRegions(parent->VirtualMemorySpace->VirtualMemoryLayout);

	PushListTail(thread, &scheduler->RunningQueue);

	return thread;
}

}