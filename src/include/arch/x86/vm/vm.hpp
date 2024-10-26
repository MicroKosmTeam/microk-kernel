#pragma once
#include <cdefs.h>

namespace x86 {
	struct VCpu {
		void *GuestVMCB;
		void *HostVMCB;
		void *SharedVMCB;
	};
}
