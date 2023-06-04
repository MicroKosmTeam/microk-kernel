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

		void SwitchToTask(size_t PID, size_t TID);
	private:
		Vector<Process*> Processes;
	};
}
