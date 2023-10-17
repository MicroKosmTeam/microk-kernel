#pragma once
#include <cstdint.hpp>

#include <mm/vmm.hpp>
#include <sys/tables.hpp>
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
		usize ID = 0;
		u8 Priority = 0;
		u16 Flags = 0;

		bool IsThread = false;
	
		ExecutableUnitHeader *Parent = NULL;

		ExecutableUnitState State;
		ExecutableUnitType Type;
	};

	struct ThreadBase : public ExecutableUnitHeader {
		CPUStatus *Context = NULL;
		uptr KernelStack = 0;

		ThreadBase *Next = NULL;
		ThreadBase *Previous = NULL;
	};

	struct ThreadList {
		usize ThreadCount = 0;
		usize ThreadIDBase = 0;

		ThreadBase *Head = NULL;
		ThreadBase *Tail = NULL;
	};

	struct ProcessBase : public ExecutableUnitHeader {
		VMM::VirtualSpace *VirtualMemorySpace = NULL;
		uptr HighestFree = 0;
		VMM::PageList *ExecutablePageList;

		ThreadList Threads;
	};

	struct KernelProcess : public ProcessBase {
	};

	struct KernelThread : public ThreadBase {
	};

	struct UserTCB : public TableHeader {
		u8 SystemTables;
		u32 SystemTableListOffset;

		u8 ServiceTables;
		u32 ServiceTableListOffset;
	}__attribute__((packed));

	struct UserProcess : public ProcessBase {
		UserTCB *UserTaskBlock = NULL;
	};

	struct UserThread : public ThreadBase {
		uptr UserStack = 0;
	};

	struct RealtimeProcess : public ProcessBase {
	};

	struct RealtimeThread : public ThreadBase {
	};

	struct VMProcess : public ProcessBase {
	};

	struct VMThread : public ThreadBase {
	};

	ProcessBase *CreateProcess(ProcessBase *parent, ExecutableUnitType type, VMM::VirtualSpace *virtualMemorySpace, VMM::PageList *pageList, u8 priority, u16 flags);
	int DeleteProcess(ProcessBase *process);

	ThreadBase *CreateThread(ProcessBase *parent, uptr entrypoint, usize stackSize, u8 priority, u16 flags);
	ThreadBase *FindThread(ProcessBase *process, usize id);
	int DeleteThread(ThreadBase *thread);

	int SetExecutableUnitState(ExecutableUnitHeader *unit, ExecutableUnitState state);

	void PopulateUserTCB(UserTCB *tcb, UserProcess *userProcess);
	void PopulateKBST(KBST *kbst);
}
