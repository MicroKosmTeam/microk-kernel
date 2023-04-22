#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/vmm.hpp>
#include <sys/vector.hpp>

namespace PROC {
	enum ProcessState {
		P_READY,		/* Process is ready to run */
		P_RUNNING,		/* Process is currently running */
		P_WAITING,		/* Process is currently waiting for I/O */
		P_PAUSED,		/* Process is not currently running */
		P_DONE,			/* Process is no longer alive */
	};

	enum ProcessType {
		PT_KERNEL,		/* Process running in kernel mode */
		PT_USER,		/* Process running in user mode */
		PT_REALTIME,		/* A realtime process running in kernel mode, has the highest priority */
		PT_VM,			/* A virtual machine process, managed by the kernel's VM manager*/
	};

	struct Process;
	struct Thread;

	/* For kernel/user processes */
	struct StandardProcess {
	};

	/* For VM processes */
	struct VMProcess {

	};

	/* For realtime processes */
	struct RTProcess {

	};

	struct Process {
		uint64_t PID;
		ProcessState State;
		ProcessType Type;
		uint8_t Priority;
		
		VMM::VirtualSpace *VirtualMemorySpace;

		uint64_t LastTID;
		size_t ThreadNumber;
		Vector<Thread*> Threads;
		Thread *MainThread;
		
		union {
			StandardProcess ProcessInfo;
			VMProcess VMProcessInfo;
			RTProcess RTProcessInfo;
		};
	};

	struct Thread {
		uint64_t TID;

		uintptr_t Stack;
		size_t StackSize;

		uintptr_t Entrypoint;
		ProcessState State;

		Process *Owner;
	};

	Process *CreateProcess(ProcessType type, VMM::VirtualSpace *vms);
	void DeleteProcess(Process *process);

	Thread *CreateThread(Process *process, uintptr_t entrypoint);
	void DeleteThread(Thread *thread);

}
