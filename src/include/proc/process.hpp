#pragma once
#include <stdint.h>
#include <stddef.h>
#include <mm/vmm.hpp>

namespace PROC {

enum ProcessState {
	P_RUNNING,		/* Process is currently running */
	P_PAUSED,		/* Process is not currently running */
};

enum ProcessType {
	PT_KERNEL,		/* Process running in kernel mode */
	PT_USER,		/* Process running in user mode */
	PT_REALTIME,		/* A realtime process running in kernel mode, has the highest priority */
	PT_VM,			/* A virtual machine process, managed by the kernel's VM manager*/
};

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

	union {
		StandardProcess ProcessInfo;
		VMProcess VMProcessInfo;
		RTProcess RTProcessInfo;
	};
};

}
