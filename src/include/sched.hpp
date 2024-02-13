#pragma once
#include <object.hpp>

namespace SCHED {
	Scheduler *InitializeCPUScheduler(Domain *domain, uptr frame);

	void AddThread(Scheduler *scheduler, ThreadControlBlock *tcb);
	void RemoveThread(Scheduler *scheduler, ThreadControlBlock *tcb);
	ThreadControlBlock *Recalculate(Scheduler *scheduler);
}
