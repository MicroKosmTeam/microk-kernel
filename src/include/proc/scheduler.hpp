#pragma once
#include <cdefs.h>
#include <cstdint.hpp>
#include <mm/slab.hpp>
#include <sys/list.hpp>
#include <proc/process.hpp>

namespace PROC {
	struct Scheduler : public ListHead {
		/* NUMA scheduling ? */
		Scheduler *ParentScheduler;

		DEV::CPU::TopologyStructure *Domain;

		MEM::SLAB::SlabCache *ProcessSlabCache;
		MEM::SLAB::SlabCache *ThreadSlabCache;

		Thread *RunningThread;

		List RunningQueue;
		List BlockedQueue;
	};

	Scheduler *InitializeScheduler(Scheduler *parent);
	void DeinitializeScheduler(Scheduler *scheduler);
	void Tick(Scheduler *scheduler, u8 *context, usize contextSize);

	Process *CreateProcess(Scheduler *scheduler, ExecutableUnitType type, VMM::VirtualSpace *space);
	Thread *CreateThread(Scheduler *scheduler, Process *parent, uptr entry);

	void SetProcessState(Scheduler *scheduler, usize pid);
	void SetThreadState(Scheduler *scheduler, usize pid, usize tid);
}
