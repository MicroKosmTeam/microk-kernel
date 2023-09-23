#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/vmm.hpp>
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
		size_t ID = 0;
		uint8_t Priority = 0;
		uint16_t Flags = 0;

		bool IsThread = false;
	
		ExecutableUnitHeader *Parent = NULL;

		ExecutableUnitState State;
		ExecutableUnitType Type;
	};

	struct ThreadBase : public ExecutableUnitHeader {
		CPUStatus *Context = NULL;
		uintptr_t KernelStack = 0;

		ThreadBase *Next = NULL;
		ThreadBase *Previous = NULL;
	};

	struct ThreadList {
		size_t ThreadCount = 0;
		size_t ThreadIDBase = 0;

		ThreadBase *Head = NULL;
		ThreadBase *Tail = NULL;
	};

	struct ProcessBase : public ExecutableUnitHeader {
		VMM::VirtualSpace *VirtualMemorySpace = NULL;
		uintptr_t HighestFree = 0;
		VMM::PageList *ExecutablePageList;

		ThreadList Threads;
	};

	struct KernelProcess : public ProcessBase {
	};

	struct KernelThread : public ThreadBase {
	};

	struct TableHeader {
		uint8_t Signature[4];
		uint8_t Revision;

		uint8_t Checksum;
	}__attribute__((packed));

	struct UserTCB : public TableHeader {
		uint8_t SystemTables;
		uint32_t SystemTableListOffset;

		uint8_t ServiceTables;
		uint32_t ServiceTableListOffset;
	}__attribute__((packed));

	struct TableListElement {
		uint8_t Signature[4];
		uintptr_t TablePointer;
	}__attribute__((packed));
	
	struct UserProcess : public ProcessBase {
		UserTCB *UserTaskBlock = NULL;
	};

	struct UserThread : public ThreadBase {
		uintptr_t UserStack = 0;
	};

	struct RealtimeProcess : public ProcessBase {
	};

	struct RealtimeThread : public ThreadBase {
	};

	struct VMProcess : public ProcessBase {
	};

	struct VMThread : public ThreadBase {
	};

	ProcessBase *CreateProcess(ProcessBase *parent, ExecutableUnitType type, VMM::VirtualSpace *virtualMemorySpace, VMM::PageList *pageList, uint8_t priority, uint16_t flags);
	int DeleteProcess(ProcessBase *process);

	ThreadBase *CreateThread(ProcessBase *parent, uintptr_t entrypoint, size_t stackSize, uint8_t priority, uint16_t flags);
	ThreadBase *FindThread(ProcessBase *process, size_t id);
	int DeleteThread(ThreadBase *thread);

	int SetExecutableUnitState(ExecutableUnitHeader *unit, ExecutableUnitState state);

	void PopulateUserTCB(UserTCB *tcb);
}
