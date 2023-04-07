#pragma once
#include <proc/process.hpp>

namespace PROC {
namespace Scheduler {
	void Initialize();
	void Pause();

	void AddCPU();
	void RemoveCPU();

	void AddProcess(Process *process, uint8_t priority);
	void RemoveProcess(uint64_t PID);

	void Cycle();

	void StartKernelThread(uintptr_t entrypoint);
}
}
