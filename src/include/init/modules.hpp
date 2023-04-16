/*
   File: include/init/modules.hpp
*/

#pragma once
#include <cdefs.h>
#ifdef CONFIG_KERNEL_MODULES

#include <init/kinfo.hpp>

namespace MODULE {
	void Init();
}

#endif
