#pragma once
#include <init/kinfo.hpp>

namespace x86_64 {
	VMM::VirtualSpace *NewVirtualSpace();
	void LoadVirtualSpace(VMM::VirtualSpace *space);
}
