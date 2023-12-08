#pragma once
#include <cstdint.hpp>
#include <dev/cpu.hpp>
#include <sys/list.hpp>
#include <mm/vmm.hpp>
#include <sys/tables.hpp>

namespace PROC {
	enum ExecutableUnitState {
		PS_RUNNING = 1,
		PS_WAITING,
		PS_BLOCKED,
		PS_DONE
	};

	enum ExecutableUnitType {
		PT_KERNEL = 1,
		PT_USER
	};

	struct UserTCB : public TableHeader {
		u8 SystemTables;
		u32 SystemTableListOffset;

		u8 ServiceTables;
		u32 ServiceTableListOffset;
	}__attribute__((packed));

	struct ExecutableUnitHeader : public ListHead {
		usize ID;
		u8 Priority;
		u16 Flags;

		bool IsThread;
	
		ExecutableUnitHeader *Parent;

		ExecutableUnitState State;
		ExecutableUnitType Type;

	};

	struct Thread : public ExecutableUnitHeader {
		usize *Context;
		DEV::CPU::TopologyStructure *RunningThread;

		uptr KernelStack;

		union {
			struct {

			} KernelThread;

			struct {
				uptr UserStack;
			} UserThread;
		};
	};

	
	struct Process: public ExecutableUnitHeader {
		DEV::CPU::TopologyStructure *Domain;

		VMM::VirtualSpace *VirtualMemorySpace;
		uptr HighestFree;

		usize ThreadCount;
		usize ThreadIDBase;

		List Threads;

		union {
			struct  {

			} KernelProcess;

			struct {
				UserTCB *UserTaskBlock;
			} UserProcess;
		};
	};
}
