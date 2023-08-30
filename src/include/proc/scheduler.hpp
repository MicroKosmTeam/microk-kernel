#pragma once
#include <proc/process.hpp>
#include <sys/vector.hpp>

namespace PROC {
	struct ProcessNode {
		ProcessBase *Proc;
		ProcessNode *Next;
	};

	class Scheduler {
	public:
		Scheduler();
		~Scheduler();

		void AddProcess(ProcessBase *process);
		void RemoveProcess(size_t PID);
		ProcessBase *GetProcess(size_t PID);
		ProcessBase *GetRunningProcess();

		void SaveProcessContext(CPUStatus *status);
		CPUStatus *GetProcessContext();
		int SetProcessState(size_t PID, ExecutableUnitState state);

		size_t GetMaxPID() { return MaxPID; }
		
			
		void RecalculateScheduler();
		void SwitchToTask(ProcessBase *proc, ThreadBase *thread);
	private:
		ProcessNode *AddNode(ProcessNode *queue, ProcessBase *proc);
		ProcessBase *PopFirstNode(ProcessNode *queue);
		ProcessBase *RemoveNode(ProcessNode *queue, size_t pid);
		ProcessNode *FindNode(ProcessNode *queue, size_t pid, ProcessNode **previous, bool *found);
		
		ProcessBase *GetProcess(size_t PID, ProcessNode *queue);

		uintmax_t MaxPID;
		ProcessBase *CurrentProcess;

		ProcessNode *RunQueueBaseNode;
		ProcessNode *BlockedQueueBaseNode;
	};
}
