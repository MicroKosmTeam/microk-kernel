#pragma once
#include <cdefs.h>
#include <arch/x86/object.hpp>

namespace x86 {
	void StartVM(uptr rip, uptr rsp, uptr rflags, uptr cr3);
}
