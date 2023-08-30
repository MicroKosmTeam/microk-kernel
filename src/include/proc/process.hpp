#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/vmm.hpp>
#include <sys/vector.hpp>
#include <arch/x64/interrupts/idt.hpp>

namespace PROC {
	enum ExecutableUnitState {
		P_READY,		/* Process is ready to run */
		P_RUNNING,		/* Process is currently running */
		P_WAITING,		/* Process is currently waiting */
		P_DONE,			/* Process is no longer alive */
		P_MESSAGE,
	};

	enum ExecutableUnitType {
		PT_KERNEL,		/* Process running in kernel mode */
		PT_USER,		/* Process running in user mode */
		PT_REALTIME,		/* A realtime process running in kernel mode, has the highest priority */
		PT_VM,			/* A virtual machine process, managed by the kernel's VM manager*/
	};

	struct ExecutableUnitHeader {
		size_t ID;
		uint8_t Priority;
		uint16_t Flags;

		bool IsThread;
	
		ExecutableUnitHeader *Parent;

		ExecutableUnitState State;
		ExecutableUnitType Type;
	};

	struct ThreadBase : public ExecutableUnitHeader {
		CPUStatus *Context;

		ThreadBase *Next;
		ThreadBase *Previous;
	};

	struct ThreadList {
		size_t ThreadCount = 0;
		size_t ThreadIDBase = 0;

		ThreadBase *Head = NULL;
		ThreadBase *Tail = NULL;
	};

	struct ProcessBase : public ExecutableUnitHeader {
		ThreadList Threads;
	};

	struct KernelProcess : public ProcessBase {
	};

	struct KernelThread : public ThreadBase {
	};

	struct UserTCB {
		
	}__attribute__((packed));

	struct UserProcess : public ProcessBase {
		VMM::VirtualSpace *VirtualMemorySpace;
		uintptr_t HighestFree;

		UserTCB *UserTaskBlock;
	};

	struct UserThread : public ThreadBase {
		uintptr_t UserStack;
		uintptr_t KernelStack;
	};

	struct RealtimeProcess : public ProcessBase {
	};

	struct RealtimeThread : public ThreadBase {
	};

	struct VMProcess : public ProcessBase {
	};

	struct VMThread : public ThreadBase {
	};

	ProcessBase *CreateProcess(ProcessBase *parent, ExecutableUnitType type, VMM::VirtualSpace *virtualMemorySpace, uint8_t priority, uint16_t flags);
	int DeleteProcess(ProcessBase *process);

	ThreadBase *CreateThread(ProcessBase *parent, uintptr_t entrypoint, size_t stackSize, uint8_t priority, uint16_t flags);
	ThreadBase *FindThread(ProcessBase *process, size_t id);
	int DeleteThread(ThreadBase *thread);

	int SetExecutableUnitState(ExecutableUnitHeader *unit, ExecutableUnitState state);


}
