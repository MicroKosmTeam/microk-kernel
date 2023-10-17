#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/panic.hpp>
#include <sys/user.hpp>
#include <sys/mutex.hpp>

namespace PROC {

Scheduler *InitializeScheduler(usize queueCount) {
	Scheduler *scheduler = (Scheduler*)Malloc(sizeof(Scheduler) + queueCount * sizeof(SchedulerQueue));

	scheduler->SchedulerLock = false;
	scheduler->CurrentThread = NULL;
	scheduler->QueueCount = queueCount;
	scheduler->ElapsedQuantum = -1;

	for (usize currentQueue = 0; currentQueue < queueCount; ++currentQueue) { 
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

	for (usize currentQueue = 0; currentQueue < scheduler->QueueCount; ++currentQueue) { 
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

int AddThreadToQueue(Scheduler *scheduler, usize queue, ThreadBase *thread) {
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
		if(oldNextNode == NULL) {
			scheduler->Queues[queue]->Tail = newNode;
		} else {
			oldNextNode->Previous = newNode;
		}

		newNode->Next = oldNextNode;
		newNode->Previous = node;
		node->Next = newNode;
	}


	UnlockMutex(&scheduler->SchedulerLock);
	return 0;
}

ThreadBase *RemoveThreadFromQueue(Scheduler *scheduler, usize queue, usize pid, usize tid) {
	if (scheduler == NULL || scheduler->Queues[queue] == NULL || scheduler->Queues[queue]->Head == NULL) return NULL;
	LockMutex(&scheduler->SchedulerLock);

	ThreadBase *thread = NULL;
	
	SchedulerNode *node = scheduler->Queues[queue]->Head;

	while(node != scheduler->Queues[queue]->Tail) {
		if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
			SchedulerNode *previous = node->Previous;
			SchedulerNode *next = node->Next;

			if(previous != NULL) {
				previous->Next = next;
			} else {
				scheduler->Queues[queue]->Head = next;
			}
			next->Previous = previous;

			thread = node->Thread;

			delete node;
		
			UnlockMutex(&scheduler->SchedulerLock);
			return thread;
		}

		node = node->Next;
	}



	if(node->Thread->Parent->ID == pid && node->Thread->ID == tid) {
		if(scheduler->Queues[queue]->Head == scheduler->Queues[queue]->Tail) {
			scheduler->Queues[queue]->Head = NULL;
			scheduler->Queues[queue]->Tail = NULL;
		} else {
			SchedulerNode *previous = node->Previous;
			previous->Next = NULL;
			scheduler->Queues[queue]->Tail = previous;
		}
			
		thread = node->Thread;
		
		delete node;
	} else {
		UnlockMutex(&scheduler->SchedulerLock);

		return NULL;
	}

	UnlockMutex(&scheduler->SchedulerLock);
	return thread;
}

ThreadBase *GetThreadFromQueue(Scheduler *scheduler, usize queue, usize pid, usize tid) {
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
	
ThreadBase *GetThread(Scheduler *scheduler, usize pid, usize tid) {
	if(scheduler == NULL) return NULL;
	LockMutex(&scheduler->SchedulerLock);

	ThreadBase *thread = NULL;
	for (usize currentQueue = 0; currentQueue < scheduler->QueueCount; ++currentQueue) { 
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
			if(scheduler->Queues[SCHEDULER_WAITING_QUEUE]->Head == NULL &&
			   scheduler->Queues[SCHEDULER_BLOCKED_QUEUE]->Head == NULL) {
				/* This is executed if there is just one process that is running
				 * we don't erase the elapsed quantum, as we want to switch to
				 * a new process as soon as we can
				 */
				UnlockMutex(&scheduler->SchedulerLock);
				return 0;
			} else {
				SchedulerQueue *oldRunning = scheduler->Queues[SCHEDULER_RUNNING_QUEUE];
				scheduler->Queues[SCHEDULER_RUNNING_QUEUE] = scheduler->Queues[SCHEDULER_WAITING_QUEUE];
				scheduler->Queues[SCHEDULER_WAITING_QUEUE] = oldRunning;
			}
		} else {
			UnlockMutex(&scheduler->SchedulerLock);
			ThreadBase *thread = RemoveThreadFromQueue(scheduler, SCHEDULER_RUNNING_QUEUE, scheduler->CurrentThread->Thread->Parent->ID, scheduler->CurrentThread->Thread->ID);
			AddThreadToQueue(scheduler, SCHEDULER_WAITING_QUEUE, thread);
			LockMutex(&scheduler->SchedulerLock);
		}
		
		scheduler->CurrentThread = scheduler->Queues[SCHEDULER_RUNNING_QUEUE]->Head;
		scheduler->ElapsedQuantum = 0;

		UnlockMutex(&scheduler->SchedulerLock);
		return 1;
	} else {
		/* Things can continue normally */	
		UnlockMutex(&scheduler->SchedulerLock);
		return 0;
	}

}

void PrintSchedulerStatus(Scheduler *scheduler) {
	if(scheduler == NULL) return;
	LockMutex(&scheduler->SchedulerLock);

	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Printing debug status of scheduler at 0x%x\r\n"
		       " Current thread:                      0x%x\r\n"
		       " Elapsed quantum:                     0x%x\r\n"
		       " Queue count:                         0x%x\r\n",
		       scheduler, scheduler->CurrentThread, scheduler->ElapsedQuantum, scheduler->QueueCount);


	ThreadBase *thread = NULL;
	for (usize currentQueue = 0; currentQueue < scheduler->QueueCount; ++currentQueue) { 
		PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "  Queue:                              0x%x\r\n", currentQueue);
		SchedulerNode *node = scheduler->Queues[currentQueue]->Head;
		if(node == NULL) PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "   Empty\r\n");
		else {
			thread = node->Thread;
			while(node != scheduler->Queues[currentQueue]->Tail) {
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "   Thread:                            0x%x\r\n"
					       "    TID:                              0x%x\r\n"
					       "    PID:                              0x%x\r\n",
					       thread, thread->ID, thread->Parent->ID);

				node = node->Next;
				thread = node->Thread;
			}

			PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "   Thread:                            0x%x\r\n"
				       "    TID:                              0x%x\r\n"
				       "    PID:                              0x%x\r\n",
				       thread, thread->ID, thread->Parent->ID);
		}

	}

	UnlockMutex(&scheduler->SchedulerLock);
}
}
