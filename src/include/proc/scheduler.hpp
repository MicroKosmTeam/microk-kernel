#pragma once
#include <proc/process.hpp>
#include <sys/vector.hpp>

namespace PROC {
	class Scheduler {
	public:
		Scheduler();
		~Scheduler();

		void AddProcess(Process *process);
		void RemoveProcess(size_t PID);
		Process *GetProcess(size_t PID);
		Process *GetRunningProcess();

		size_t GetMaxPID() { return MaxPID; }
			
		void SwitchToTask(size_t PID, size_t TID);
	private:
		uintmax_t MaxPID;
		Process *CurrentProcess;

		Vector<Process*> Processes;
	};
}
