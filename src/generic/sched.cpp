#include <sched.hpp>
#include <list.hpp>
#include <memory.hpp>

namespace SCHED {
Scheduler *InitializeCPUScheduler(Domain *domain, uptr frame) {
	Scheduler *scheduler = (Scheduler*)frame;
	Memclr(scheduler, sizeof(Scheduler));
	
	scheduler->Parent = domain;
	scheduler->Running = NULL;

	domain->DomainScheduler = scheduler;

	return scheduler;
}

void AddThread(Scheduler *scheduler, ThreadControlBlock *tcb) {
	switch (tcb->Status) {
		case ThreadStatus::RUNNING:
			/* Invalid, can't do that. Treat it as a WAITING task. */
		        __attribute__ ((fallthrough));
		case ThreadStatus::WAITING:
			AddElementToList(tcb, &scheduler->Waiting[tcb->Priority]);
			break;
		default:
			/* Unknown status? Treat it as if the thread isn't fully initialized
			 * and isn't ready to run. Blocked.
			 */
		        __attribute__ ((fallthrough));
		case ThreadStatus::BLOCKED:
			AddElementToList(tcb, &scheduler->Blocked[tcb->Priority]);
			break;
	}

}

void RemoveThread(Scheduler *scheduler, ThreadControlBlock *tcb) {
	switch (tcb->Status) {
		case ThreadStatus::RUNNING:
			scheduler->Running = NULL;
		        __attribute__ ((fallthrough));
		case ThreadStatus::WAITING:
			RemoveElementFromList(tcb, &scheduler->Waiting[tcb->Priority]);
			break;
		case ThreadStatus::BLOCKED:
			RemoveElementFromList(tcb, &scheduler->Blocked[tcb->Priority]);
			break;
		default:
			break;
	}
}

ThreadControlBlock *Recalculate(Scheduler *scheduler) {
	if (scheduler->Running != NULL) {
		AddElementToList(scheduler->Running, &scheduler->Waiting[scheduler->Running->Priority]);
	}

	for (u8 prio = SCHEDULER_MAX_PRIORITY; prio < SCHEDULER_MIN_PRIORITY; ++prio) {
		scheduler->Running = (ThreadControlBlock*)PopListHead(&scheduler->Waiting[prio]);

		if (scheduler->Running) {
			break;
		}
	}

	return scheduler->Running;
}

}
