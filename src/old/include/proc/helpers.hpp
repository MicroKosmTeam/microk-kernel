#pragma once
#include <sys/locks.hpp>
#include <init/kinfo.hpp>

namespace PROC {
	inline PROC::Process *GetProcess() {
		return NULL;
	}

	inline VMM::VirtualSpace *GetVirtualSpace(PROC::Process *proc) {
		(void)proc;
		return NULL;
	}
}
