#pragma once
#include <proc/process.hpp>
#include <sys/vector.hpp>

namespace PROC {
	struct ProcessNode {
		Process *Proc;
		ProcessNode *Next;
	};

	class Scheduler {
	public:
		Scheduler();
		~Scheduler();

		void AddProcess(Process *process);
		void RemoveProcess(size_t PID);
		Process *GetProcess(size_t PID);
		Process *GetRunningProcess();

		void SaveProcessContext(CPUStatus *status);
		CPUStatus *GetProcessContext();
		int SetProcessState(size_t PID, ProcessState state);

		size_t GetMaxPID() { return MaxPID; }
		
			
		void RecalculateScheduler();
		void SwitchToTask(Process *proc, Thread *thread);
	private:
		ProcessNode *Scheduler::AddNode(ProcessNode *queue, Process *proc);
		Process *PopFirstNode(ProcessNode *queue);
		Process *RemoveNode(ProcessNode *queue, size_t pid);
		ProcessNode *FindNode(ProcessNode *queue, size_t pid, ProcessNode **previous, bool *found);
		
		Process *GetProcess(size_t PID, ProcessNode *queue);

		uintmax_t MaxPID;
		Process *CurrentProcess;

		ProcessNode *RunQueueBaseNode;
		ProcessNode *BlockedQueueBaseNode;
	};
}
