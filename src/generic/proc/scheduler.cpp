#include <proc/scheduler.hpp>
#include <arch/x64/cpu/stack.hpp>
#include <sys/printk.hpp>
#include <init/kinfo.hpp>
#include <mm/pmm.hpp>
#include <sys/panic.hpp>
#include <sys/user.hpp>

namespace PROC {

ProcessNode *Scheduler::AddNode(ProcessNode *queue, Process *proc) {
	bool found = false;
	ProcessNode *node, *prev;

	node = FindNode(queue, proc->GetPID(), &prev, &found);

	/* Already present, return this one */
	if(found) return node;

	/* If not, prev is now our last node. */
	ProcessNode *newNode = new ProcessNode;
	newNode->Proc = proc;
	newNode->Next = NULL;

	prev->Next = newNode;
	
	return prev->Next;
}

Process *Scheduler::PopFirstNode(ProcessNode *queue) {
	ProcessNode *node = queue->Next;

	if (node == NULL) {
		return NULL;
	}

	queue->Next = node->Next;

	Process *proc = node->Proc;
	delete node;

	return proc;
}

Process *Scheduler::RemoveNode(ProcessNode *queue, size_t pid) {
	bool found = false;
	ProcessNode *previous; 
	ProcessNode *node = FindNode(queue, pid, &previous, &found);

	/* This issue shall be reported */
	if(!found) return NULL;

	previous->Next = node->Next;
	Process *proc = node->Proc;
	delete node;

	return proc;
}

ProcessNode *Scheduler::FindNode(ProcessNode *queue, size_t pid, ProcessNode **previous, bool *found) {
	ProcessNode *node = queue->Next, *prev = queue;

	if (node == NULL) {
		*previous = prev;
		*found = false;
		return NULL;
	}

	while(true) {
		if (node->Proc->GetPID() == pid) {
			*found = true;
			*previous = prev;
			return node;
		}

		prev = node;
		if (node->Next == NULL) break;
		node = node->Next;
	}

	*previous = prev;
	*found = false;
	return NULL;
}

Scheduler::Scheduler() {
	KInfo *info = GetInfo();

	CurrentProcess = NULL;
	
	RunQueueBaseNode = new ProcessNode;
	BlockedQueueBaseNode = new ProcessNode;

	RunQueueBaseNode->Proc = NULL;
	RunQueueBaseNode->Next = NULL;

	BlockedQueueBaseNode->Proc = NULL;
	BlockedQueueBaseNode->Next = NULL;

	info->kernelProcess = new Process(PT_KERNEL, info->kernelVirtualSpace);
	AddProcess(info->kernelProcess);
}

void Scheduler::AddProcess(Process *process) {
	if (process == NULL) return;

	AddNode(BlockedQueueBaseNode, process);

	if (process->GetPID() > MaxPID) MaxPID = process->GetPID();
}

int Scheduler::SetProcessState(size_t PID, ProcessState state) {
	Process *proc;

	switch(state) {
		case P_READY:
			proc = GetProcess(PID, BlockedQueueBaseNode);
			if (proc == NULL) return -1;

			RemoveNode(BlockedQueueBaseNode, PID);
			AddNode(RunQueueBaseNode, proc);

			break;
		case P_RUNNING:
			proc = GetProcess(PID, RunQueueBaseNode);
			if (proc == NULL) return -1;

			CurrentProcess = proc;

			break;
		case P_WAITING:
			proc = GetProcess(PID, RunQueueBaseNode);
			if (proc == NULL) return -1;

			RemoveNode(RunQueueBaseNode, PID);
			AddNode(BlockedQueueBaseNode, proc);

			break;
		case P_MESSAGE:
			proc = GetProcess(PID, BlockedQueueBaseNode);
			if (proc == NULL) return -1;

			RemoveNode(BlockedQueueBaseNode, PID);
			AddNode(RunQueueBaseNode, proc);

			break;
		case P_DONE:
			proc = GetProcess(PID);
			if (proc == NULL) return -1;

			if(RemoveNode(RunQueueBaseNode, PID) == NULL)
				if (RemoveNode(BlockedQueueBaseNode, PID) == NULL)
					return -1;

			break;
	}

	proc->SetProcessState(state);

	return 0;
}

Process *Scheduler::GetProcess(size_t PID, ProcessNode *queue) {
	if(PID > MaxPID) return NULL;

	bool found = false;
	ProcessNode *previous; 
	ProcessNode *node = FindNode(queue, PID, &previous, &found);

	if(found) return node->Proc;
	
	return NULL;
}

Process *Scheduler::GetProcess(size_t PID) {
	if(PID > MaxPID) return NULL;

	bool found = false;
	ProcessNode *previous; 
	ProcessNode *node = FindNode(RunQueueBaseNode, PID, &previous, &found);

	if(found) return node->Proc;
	
	node = FindNode(BlockedQueueBaseNode, PID, &previous, &found);

	if (found) return node->Proc;
	
	return NULL;
}

Process *Scheduler::GetRunningProcess() {
	return CurrentProcess;
}

void Scheduler::RecalculateScheduler() {
	PROC::Process *proc = PopFirstNode(RunQueueBaseNode);
	if (proc == NULL) return;

	AddNode(RunQueueBaseNode, proc);

	if (RunQueueBaseNode->Next == NULL) return;

	CurrentProcess = proc;

	SwitchToTask(RunQueueBaseNode->Next->Proc, 0);
}

void Scheduler::SwitchToTask(Process *proc, size_t TID) {
	if (proc == NULL) return;

	Thread *thread;

	if (TID == 0) thread = proc->GetMainThread();
	else thread = proc->GetThread(TID);
	
	if (thread == NULL) return;
	
	if(proc->GetProcessState() == P_MESSAGE) {
		Thread *msg = proc->GetMessageThread();
		if (msg != NULL) thread = msg;
	}

	void *stack = thread->GetStack();
	void *entry = thread->GetInstruction();

	CurrentProcess = proc;

	ProcessType type = proc->GetType();

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

}
