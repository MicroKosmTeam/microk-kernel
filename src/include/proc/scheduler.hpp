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
	};

	Scheduler *InitializeScheduler();
	void DeinitializeScheduler(Scheduler *scheduler);

	Process *CreateProcess(Scheduler *scheduler, ExecutableUnitType type, VMM::VirtualSpace *space);
	Thread *CreateThread(Scheduler *scheduler, Process *parent);
}
