#pragma once
#include <sys/locks.hpp>
#include <init/kinfo.hpp>

namespace PROC {
	inline PROC::ProcessBase *GetProcess() {
		KInfo *info = GetInfo();

		SpinlockLock(&info->KernelScheduler->SchedulerLock);

		PROC::SchedulerNode *currentNode = info->KernelScheduler->CurrentThread;
		if (currentNode == NULL) {
			return NULL;
		}

		PROC::ThreadBase *thread = currentNode->Thread;
		if (thread == NULL) {
			return NULL;
		}

		PROC::ProcessBase *proc = (PROC::ProcessBase*)thread->Parent;
		SpinlockUnlock(&info->KernelScheduler->SchedulerLock);

		return proc;
	}

	inline uptr GetVirtualSpace(PROC::ProcessBase *proc) {
		KInfo *info = GetInfo();

		SpinlockLock(&info->KernelScheduler->SchedulerLock);
		uptr procSpace = proc->VirtualMemorySpace;
		SpinlockUnlock(&info->KernelScheduler->SchedulerLock);

		return procSpace;
	}

	inline VMM::PageList *GetPageList(PROC::ProcessBase *proc) {
		KInfo *info = GetInfo();

		SpinlockLock(&info->KernelScheduler->SchedulerLock);
		VMM::PageList *procList = proc->ExecutablePageList;
		SpinlockUnlock(&info->KernelScheduler->SchedulerLock);

		return procList;
	}
}
