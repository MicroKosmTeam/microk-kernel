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

	#define SCHEDULER_DEFAULT_QUANTUM 1

	struct SchedulerNode {
		ThreadBase *Thread = NULL;
		size_t Quantum = 0;

		SchedulerNode *Next = NULL;
		SchedulerNode *Previous = NULL;
	};

	struct SchedulerQueue {
		size_t ThreadCount = 0;

		SchedulerNode *Head = NULL;
		SchedulerNode *Tail = NULL;
	};

	struct Scheduler {
		SchedulerNode *CurrentThread = NULL;

		size_t ElapsedQuantum = 0;

		bool SchedulerLock = false;

		size_t QueueCount = 0;
		SchedulerQueue *Queues[];
	};

	Scheduler *InitializeScheduler(size_t queueCount);
	int DeinitializeScheduler(Scheduler *scheduler);

	int AddThreadToQueue(Scheduler *scheduler, size_t queue, ThreadBase *thread);
	ThreadBase *RemoveThreadFromQueue(Scheduler *scheduler, size_t queue, size_t pid, size_t tid);
	ThreadBase *GetThreadFromQueue(Scheduler *scheduler, size_t queue, size_t pid, size_t tid);
	ThreadBase *GetThread(Scheduler *scheduler, size_t pid, size_t tid);

	int RecalculateScheduler(Scheduler *scheduler);

	void PrintSchedulerStatus(Scheduler *scheduler);
}
