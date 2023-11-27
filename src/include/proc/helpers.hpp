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

	inline VMM::VirtualSpace *GetVirtualSpace(PROC::ProcessBase *proc) {
		KInfo *info = GetInfo();

		SpinlockLock(&info->KernelScheduler->SchedulerLock);
		VMM::VirtualSpace *procSpace = proc->VirtualMemorySpace;
		SpinlockUnlock(&info->KernelScheduler->SchedulerLock);

		return procSpace;
	}
}
