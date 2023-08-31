#pragma once
#include <cdefs.h>
#include <stdint.h>
#include <stddef.h>
#include <proc/process.hpp>

namespace PROC {
	#define SCHEDULER_DEFAULT_QUEUES 3

	#define SCHEDULER_RUNNING_QUEUE  0
	#define SCHEDULER_WAITING_QUEUE  1
	#define SCHEDULER_BLOCKED_QUEUE  2

	#define SCHEDULER_DEFAULT_QUANTUM 5

	struct SchedulerNode {
		ThreadBase *Thread;
		size_t Quantum;

		SchedulerNode *Next;
		SchedulerNode *Previous;
	};

	struct SchedulerQueue {
		size_t ThreadCount;

		SchedulerNode *Head;
		SchedulerNode *Tail;
	};

	struct Scheduler {
		SchedulerNode *CurrentThread;

		size_t ElapsedQuantum;

		bool SchedulerLock;

		size_t QueueCount;
		SchedulerQueue *Queues[];
	};

	Scheduler *InitializeScheduler(size_t queueCount);
	int DeinitializeScheduler(Scheduler *scheduler);

	int AddThreadToQueue(Scheduler *scheduler, size_t queue, ThreadBase *thread);
	ThreadBase *RemoveThreadFromQueue(Scheduler *scheduler, size_t queue, size_t pid, size_t tid);
	ThreadBase *GetThreadFromQueue(Scheduler *scheduler, size_t queue, size_t pid, size_t tid);
	ThreadBase *GetThread(Scheduler *scheduler, size_t pid, size_t tid);

	int RecalculateScheduler(Scheduler *scheduler);
}
