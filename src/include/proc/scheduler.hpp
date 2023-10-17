#pragma once
#include <cdefs.h>
#include <cstdint.hpp>

#include <proc/process.hpp>

namespace PROC {
	#define SCHEDULER_DEFAULT_QUEUES 3

	#define SCHEDULER_RUNNING_QUEUE  0
	#define SCHEDULER_WAITING_QUEUE  1
	#define SCHEDULER_BLOCKED_QUEUE  2

	#define SCHEDULER_DEFAULT_QUANTUM 1

	struct SchedulerNode {
		ThreadBase *Thread = NULL;
		usize Quantum = 0;

		SchedulerNode *Next = NULL;
		SchedulerNode *Previous = NULL;
	};

	struct SchedulerQueue {
		usize ThreadCount = 0;

		SchedulerNode *Head = NULL;
		SchedulerNode *Tail = NULL;
	};

	struct Scheduler {
		SchedulerNode *CurrentThread = NULL;

		usize ElapsedQuantum = 0;

		bool SchedulerLock = false;

		usize QueueCount = 0;
		SchedulerQueue *Queues[];
	};

	Scheduler *InitializeScheduler(usize queueCount);
	int DeinitializeScheduler(Scheduler *scheduler);

	int AddThreadToQueue(Scheduler *scheduler, usize queue, ThreadBase *thread);
	ThreadBase *RemoveThreadFromQueue(Scheduler *scheduler, usize queue, usize pid, usize tid);
	ThreadBase *GetThreadFromQueue(Scheduler *scheduler, usize queue, usize pid, usize tid);
	ThreadBase *GetThread(Scheduler *scheduler, usize pid, usize tid);

	int RecalculateScheduler(Scheduler *scheduler);

	void PrintSchedulerStatus(Scheduler *scheduler);
}
