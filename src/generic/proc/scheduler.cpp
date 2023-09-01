#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/panic.hpp>
#include <sys/user.hpp>
#include <sys/mutex.hpp>

namespace PROC {

Scheduler *InitializeScheduler(size_t queueCount) {
	Scheduler *scheduler = (Scheduler*)Malloc(sizeof(Scheduler) + queueCount * sizeof(SchedulerQueue));

	scheduler->SchedulerLock = false;
	scheduler->CurrentThread = NULL;
	scheduler->QueueCount = queueCount;
	scheduler->ElapsedQuantum = -1;

	for (size_t currentQueue = 0; currentQueue < queueCount; ++currentQueue) { 
		scheduler->Queues[currentQueue] = new SchedulerQueue;
		scheduler->Queues[currentQueue]->ThreadCount = 0;
		scheduler->Queues[currentQueue]->Head = NULL;
		scheduler->Queues[currentQueue]->Tail = NULL;
	}

	return scheduler;
}

int DeinitializeScheduler(Scheduler *scheduler) {
	if(scheduler == NULL) return -1;
	LockMutex(&scheduler->SchedulerLock);

	for (size_t currentQueue = 0; currentQueue < scheduler->QueueCount; ++currentQueue) { 
		if(scheduler->Queues[currentQueue]->Head != NULL) {
			SchedulerNode *node = scheduler->Queues[currentQueue]->Head;
			while(node != scheduler->Queues[currentQueue]->Tail) {
				node = node->Next;
				delete node->Previous;
			}

			delete scheduler->Queues[currentQueue]->Tail;
		}

		delete scheduler->Queues[currentQueue];
	}

	Free(scheduler);

	return 0;
}

int AddThreadToQueue(Scheduler *scheduler, size_t queue, ThreadBase *thread) {
	if (scheduler == NULL || scheduler->Queues[queue] == NULL || thread == NULL) return -1;
	LockMutex(&scheduler->SchedulerLock);

	SchedulerNode *newNode = new SchedulerNode;
	newNode->Thread = thread;
	newNode->Quantum = SCHEDULER_DEFAULT_QUANTUM;
	newNode->Next = NULL;
	newNode->Previous = NULL;

	if(scheduler->Queues[queue]->Head == NULL) {
		scheduler->Queues[queue]->Head = newNode;
		scheduler->Queues[queue]->Tail = newNode;
	} else {
		SchedulerNode *node = scheduler->Queues[queue]->Head;
		while(node != scheduler->Queues[queue]->Tail) {
			if(node->Thread->Priority < thread->Priority)
				break;

			node = node->Next;
		}

		SchedulerNode *oldNextNode = node->Next;
		newNode->Next = oldNextNode;
		newNode->Previous = node;
		node->Next = newNode;
	}


	UnlockMutex(&scheduler->SchedulerLock);
	return 0;
}

ThreadBase *RemoveThreadFromQueue(Scheduler *scheduler, size_t queue, size_t pid, size_t tid) {
	if (scheduler == NULL || scheduler->Queues[queue] == NULL || scheduler->Queues[queue]->Head == NULL) return NULL;
	LockMutex(&scheduler->SchedulerLock);

	ThreadBase *thread = NULL;

	SchedulerNode *node = scheduler->Queues[queue]->Head;
	while(node != scheduler->Queues[queue]->Tail) {
		if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
			SchedulerNode *previous = node->Previous;
			SchedulerNode *next = node->Next;

			previous->Next = next;
			next->Previous = previous;

			thread = node->Thread;

			delete node;
		
			UnlockMutex(&scheduler->SchedulerLock);
			return thread;
		}

		node = node->Next;
	}

	if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
		SchedulerNode *previous = node->Previous;
		previous->Next = NULL;
		scheduler->Queues[queue]->Tail = previous;
			
		thread = node->Thread;
		
		delete node;
	} else {
		UnlockMutex(&scheduler->SchedulerLock);

		return NULL;
	}

	UnlockMutex(&scheduler->SchedulerLock);
	return thread;
}

ThreadBase *GetThreadFromQueue(Scheduler *scheduler, size_t queue, size_t pid, size_t tid) {
	if (scheduler == NULL || scheduler->Queues[queue] == NULL || scheduler->Queues[queue]->Head == NULL) return NULL;
	LockMutex(&scheduler->SchedulerLock);

	ThreadBase *thread = NULL;

	SchedulerNode *node = scheduler->Queues[queue]->Head;
	while(node != scheduler->Queues[queue]->Tail) {
		if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
			thread = node->Thread;
			UnlockMutex(&scheduler->SchedulerLock);
			return thread;
		}

		node = node->Next;
	}

	if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
		thread = node->Thread;
	} else {
		UnlockMutex(&scheduler->SchedulerLock);

		return NULL;
	}

	UnlockMutex(&scheduler->SchedulerLock);
	return thread;
}
	
ThreadBase *GetThread(Scheduler *scheduler, size_t pid, size_t tid) {
	if(scheduler == NULL) return NULL;
	LockMutex(&scheduler->SchedulerLock);

	ThreadBase *thread = NULL;
	for (size_t currentQueue = 0; currentQueue < scheduler->QueueCount; ++currentQueue) { 
		SchedulerNode *node = scheduler->Queues[currentQueue]->Head;
		while(node != scheduler->Queues[currentQueue]->Tail) {
			if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
				thread = node->Thread;
				UnlockMutex(&scheduler->SchedulerLock);
				return thread;
			}

			node = node->Next;
		}

		if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
			thread = node->Thread;
			UnlockMutex(&scheduler->SchedulerLock);
			return thread;
		}
	}

	UnlockMutex(&scheduler->SchedulerLock);
	return NULL;
}

int RecalculateScheduler(Scheduler *scheduler) {
	if(scheduler == NULL) return -1;
	LockMutex(&scheduler->SchedulerLock);

	/* Check wether the current thread has used up its available quantum */
	if(scheduler->CurrentThread == NULL) {
		if(scheduler->Queues[SCHEDULER_RUNNING_QUEUE]->Head != NULL) {
			scheduler->CurrentThread = scheduler->Queues[SCHEDULER_RUNNING_QUEUE]->Head;
			scheduler->ElapsedQuantum = 0;
			UnlockMutex(&scheduler->SchedulerLock);
			return 0;
		} else {
			UnlockMutex(&scheduler->SchedulerLock);
			return -1;
		}
	}

	if(scheduler->ElapsedQuantum >= scheduler->CurrentThread->Quantum) {
		/* If there are no threads in the running queue after this one, we'll swap it with the waiting queue 
		 * Otherwise, we'll just remove it and push it in the waiting queue */
		
		if(scheduler->Queues[SCHEDULER_RUNNING_QUEUE]->Head->Next == NULL) {
			SchedulerQueue *oldRunning = scheduler->Queues[SCHEDULER_RUNNING_QUEUE];
			scheduler->Queues[SCHEDULER_RUNNING_QUEUE] = scheduler->Queues[SCHEDULER_WAITING_QUEUE];
			scheduler->Queues[SCHEDULER_WAITING_QUEUE] = oldRunning;
		} else {
			ThreadBase *thread = RemoveThreadFromQueue(scheduler, SCHEDULER_WAITING_QUEUE, scheduler->CurrentThread->Thread->Parent->ID, scheduler->CurrentThread->Thread->ID);
			AddThreadToQueue(scheduler, SCHEDULER_WAITING_QUEUE, thread);
		}
		
		scheduler->CurrentThread = scheduler->Queues[SCHEDULER_RUNNING_QUEUE]->Head;

		UnlockMutex(&scheduler->SchedulerLock);
		return 1;
	} else {
		/* Things can continue normally */	
		UnlockMutex(&scheduler->SchedulerLock);
		return 0;
	}

}

/*
void Scheduler::SwitchToTask(Process *proc, Thread *thread) {
	if (proc == NULL) return;
	if (thread == NULL) return;

	volatile void *stack = thread->GetStack();
	volatile void *entry = thread->GetInstruction();

	ProcessType type = proc->GetType();
		
	CurrentProcess = proc;

	VMM::LoadVirtualSpace(proc->GetVirtualMemorySpace());

	switch(type) {
		case PT_USER:
			EnterUserspace(entry, stack);
			break;
		case PT_KERNEL:
			uint64_t old;
			SaveContext *context = stack;
			context->ret = entry;
			SwitchStack(&old, &stack);
			break;
	}

	OOPS("Failed to task switch");
}
*/
}
