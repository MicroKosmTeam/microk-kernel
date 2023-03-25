/*
   File: include/init/modules.hpp
*/

#pragma once
#include <init/kinfo.hpp>

/*
    The kernel symble table that contains the addresses of functions available to
    fully kernel mode drivers
*/
extern uint64_t *KRNLSYMTABLE;

namespace MODULE {
	void Init();
}
