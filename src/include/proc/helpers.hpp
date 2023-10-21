#pragma once
#include <init/kinfo.hpp>

namespace PROC {
	inline PROC::ProcessBase *GetProcess() {
		KInfo *info = GetInfo();

		LockMutex(&info->KernelScheduler->SchedulerLock);
		PROC::ProcessBase *proc = (PROC::ProcessBase*)info->KernelScheduler->CurrentThread->Thread->Parent;
		UnlockMutex(&info->KernelScheduler->SchedulerLock);

		return proc;
	}

	inline VMM::VirtualSpace *GetVirtualSpace(PROC::ProcessBase *proc) {
		KInfo *info = GetInfo();

		LockMutex(&info->KernelScheduler->SchedulerLock);
		VMM::VirtualSpace *procSpace = proc->VirtualMemorySpace;
		UnlockMutex(&info->KernelScheduler->SchedulerLock);

		return procSpace;
	}

	inline VMM::PageList *GetPageList(PROC::ProcessBase *proc) {
		KInfo *info = GetInfo();

		LockMutex(&info->KernelScheduler->SchedulerLock);
		VMM::PageList *procList = proc->ExecutablePageList;
		UnlockMutex(&info->KernelScheduler->SchedulerLock);

		return procList;
	}
}
