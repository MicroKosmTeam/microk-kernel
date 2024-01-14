#include <sched.hpp>
#include <list.hpp>

namespace SCHED {
Scheduler *InitializeCPUScheduler(Domain *domain) {
	(void)domain;
	return NULL;
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
			/* We need to find another thread to substitute this one first */
			Recalculate(scheduler);

			if (scheduler->Running == tcb) {
				/* Only one thread we have, yeet it with prejudice */
				scheduler->Running = NULL;
			}
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
	AddElementToList(scheduler->Running, &scheduler->Waiting[scheduler->Running->Priority]);

	for (u8 prio = SCHEDULER_MAX_PRIORITY; prio < SCHEDULER_MIN_PRIORITY; ++prio) {
		scheduler->Running = (ThreadControlBlock*)PopListHead(&scheduler->Waiting[prio]);

		if (scheduler->Running) {
			break;
		}
	}

	return scheduler->Running;
}

}
